#pragma once
#include "stdafx.h"
#include "protocol_srtuctures.h"

namespace ver1 {

/// <summary>
///   Simba Packet accepted to process
/// </summary>
/// <param name="src">Simba Packet</param>
/// <returns>EXIT_SUCCESS or EXIT_FAILURE</returns>
inline
int ProcessSimbaPacket(std::span<const unsigned char> simbaPacket) {
    const MDPH_ForPacket* pHeader = reinterpret_cast<const MDPH_ForPacket*>(simbaPacket.data());

    if (pHeader->MsgSize != simbaPacket.size()) [[unlikely]] {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/// <summary>
///   we almost near the data
/// </summary>
/// <param name="src">UDP header and data after it</param>
/// <returns>EXIT_SUCCESS or EXIT_FAILURE</returns>
inline
int ProcessUDPHeader(std::span<const unsigned char> src) {
    const udp_header* pUDPHeader = reinterpret_cast<const udp_header*>(src.data());

    const uint16_t udpLength = htons(pUDPHeader->len);
    if (udpLength != src.size()) [[unlikely]] {
        return EXIT_FAILURE;
    }

    ProcessSimbaPacket({src.data() + sizeof(udp_header), udpLength - sizeof(udp_header)});

    return EXIT_SUCCESS;
}


/// <summary>
/// 
/// </summary>
/// <param name="src"></param>
/// <returns>EXIT_SUCCESS or EXIT_FAILURE</returns>
inline
int ProcessIPHeader(std::span<const unsigned char> src) {
    const ip_header* pIPHeader = reinterpret_cast<const ip_header*>(src.data());

    if (pIPHeader->lengthBE() != src.size()) [[unlikely]] {
        return EXIT_FAILURE;
    }


    if (UDP_PROTOCOL == pIPHeader->protocol) {
        uint64_t offset = pIPHeader->ihl * 4;
        return ProcessUDPHeader({src.data() + offset,  src.size() - offset});
    }

    return EXIT_SUCCESS;
}



/// <summary>
///  the main logic happens here
/// </summary>
/// <param name="src">data to process</param>
/// <returns>EXIT_FAILURE or EXIT_SUCCESS</returns>
int ProcessTheStream(std::span<const unsigned char> src) {
    if ((reinterpret_cast<const pcapFileHeader*>(src.data()))->link_type != 0) [[unlikely]] {
        return EXIT_FAILURE;
    }

    std::span<const unsigned char> pkt = src.subspan(sizeof(pcapFileHeader));
    uint64_t offset = sizeof(pcapFileHeader); // full offset

    std::size_t nPackets = 0;
    while (offset + sizeof(pcapRecord) < src.size()) [[likely]] {
        const pcapRecord* pRecord = reinterpret_cast<const pcapRecord*>(pkt.data());
        const uint32_t lenH = pRecord->length_captured;
        if (lenH < sizeof(pcapEthernetII)) [[unlikely]] {
            break;
        }
        const pcapEthernetII* pEthernetII = reinterpret_cast<const pcapEthernetII*>(pkt.data() + sizeof(pcapRecord));
        if (pEthernetII->version != 0x0008) [[unlikely]] {// IPv4 
            break;
        }
        // place to split on streams


        if (EXIT_SUCCESS != ProcessIPHeader(
                {src.data() + offset + sizeof(pcapRecord) + sizeof(pcapEthernetII), lenH - sizeof(pcapEthernetII)})) [[unlikely]] {
            break;
        }

        offset += lenH + sizeof(pcapRecord);
        ++nPackets;
        pkt = src.subspan(offset);
    }

    std::cout << "Found " << nPackets << " packets\n";
    return (offset != src.size()) * EXIT_FAILURE;
}



};

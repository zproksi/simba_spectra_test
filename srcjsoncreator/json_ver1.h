#pragma once
#include "stdafx.h"
#include "formatting.h"
#include "protocol_srtuctures.h"

namespace ver1 {

/// <summary>
///   Snapshot Packet to process
/// </summary>
/// <param name="pHeader">Simba Packet</param>
/// <returns>NA</returns>
inline
void ProcessSnapshot(const MDPH_ForPacket* pHeader) {
    char date[64] = {0};
    formatting::formatTimeUTC(date, pHeader->sendingTime)[0] = '\0';

    std::cout << "Snapshot sent time is " << date << "\n";
    [[maybe_unused]]
    const SBE_MessageHeader* const pSBEHeader = reinterpret_cast<const SBE_MessageHeader*>(pHeader)
        + sizeof(*pHeader);
}


/// <summary>
///   doing SBEMessages
/// </summary>
/// <param name="incrementalData">data with SBE Headers + Root blocks</param>
inline
void ProcessIncrementalMessages(const std::span<const unsigned char> incrementalData) {
    size_t sz = 0;

    while (sz < incrementalData.size())
    {
        const unsigned char* const p = incrementalData.data() + sz;
        const SBE_MessageHeader* pSBEHeader = reinterpret_cast<const SBE_MessageHeader*>(p);

        std::span<const unsigned char> rootblock{p + sizeof(*pSBEHeader), pSBEHeader->BlockLength};
        const uint16_t* const pMsgID = reinterpret_cast<const uint16_t*>(rootblock.data());

        sz += sizeof(*pSBEHeader) + pSBEHeader->BlockLength;
    }
}



/// <summary>
///   Incremental Packet to process
/// </summary>
/// <param name="pHeader">Simba Packet</param>
/// <returns>NA</returns>
inline
void ProcessIncremental(const MDPH_ForPacket* const pHeader, const IncrementalPacketHeader* const pIncremental) {
    char date[64] = {0};
    formatting::formatTimeUTC(date, pHeader->sendingTime)[0] = '\0';
//    char transactionDate[64] = {0};
//    formatting::formatTimeUTC(transactionDate, pIncremental->TransactTime)[0] = '\0';
//    "   Transaction date is " << transactionDate <<
    std::cout << "Incremental sent time is " << date << "\n";

    ProcessIncrementalMessages(
        {reinterpret_cast<const unsigned char*>(pHeader) + sizeof(*pHeader) + sizeof(*pIncremental),
        pHeader->msgSize - sizeof(*pHeader) - sizeof(*pIncremental)}
    );
}


    /// <summary>
///   Simba Packet accepted to process
/// </summary>
/// <param name="simbaPacket">Simba Packet</param>
/// <returns>EXIT_SUCCESS or EXIT_FAILURE</returns>
inline
int ProcessSimbaPacket(std::span<const unsigned char> simbaPacket) {
    const MDPH_ForPacket* pHeader = reinterpret_cast<const MDPH_ForPacket*>(simbaPacket.data());

    if (pHeader->msgSize != simbaPacket.size()) [[unlikely]] {
        return EXIT_FAILURE;
    }


    if (pHeader->TestFlag(MDH_MsgFlags::IncrementalPacket)) {
        ProcessIncremental(pHeader, 
            reinterpret_cast<const IncrementalPacketHeader*>(simbaPacket.data() + sizeof(*pHeader)));
    } else {
        ProcessSnapshot(pHeader);
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

#pragma once

constexpr uint8_t UDP_PROTOCOL = 17;

#pragma pack(push, 1)

struct pcapFileHeader
{
    uint32_t magic;
    uint16_t major;
    uint16_t minor;
    uint32_t reserved1;
    uint32_t reserved2;
    uint32_t snap_length;
    uint16_t FCS_f_rest;
    uint16_t link_type;
};
static_assert(sizeof(pcapFileHeader) == 24, "must be 24");

struct pcapRecord
{
    uint32_t timestamp_seconds;
    uint32_t timestamp_micro_nano;
    uint32_t length_captured;
    uint32_t length_original;
};
static_assert(sizeof(pcapRecord) == 16, "must be 24");


struct pcapEthernetII
{
    uint8_t destination[6];
    uint8_t source[6];
    uint16_t version;
};
static_assert(sizeof(pcapEthernetII) == 14, "must be 14");

struct ip_header
{
//    uint8_t ihlVer;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t ihl : 4;
    uint8_t version : 4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t version : 4;
    uint8_t ihl : 4;
#else
# error "Please fix <bits/endian.h>"
#endif
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
    /*The options start here. */

    //uint8_t ihl() const
    //{
    //    return (ihlVer & (uint8_t)(0x0F));
    //}
    //uint8_t Version() const
    //{
    //    return (ihlVer & (uint8_t)(0xF0)) >> 4;
    //}

    uint16_t lengthLE() const
    {
        return tot_len;
    }
    uint16_t lengthBE() const
    {
        return htons(tot_len);
    }
};

static_assert(sizeof(ip_header) == 20, "must be 20");

struct udp_header
{
    uint16_t source;
    uint16_t dest;
    uint16_t len;
    uint16_t check;
};



///Market Data Packet Header
struct MDPH_ForPacket {
    uint32_t MsgSeqNum;
    uint16_t MsgSize; // including this header
    uint16_t MsgFlags;
    uint64_t SendingTime; //In nanoseconds
};

/// SBE Message Header
struct SBE_MessageHeader {
    uint16_t BlockLength;
    uint16_t TemplateID;
    uint16_t SchemaID;
    uint16_t Version;
};

/// Repeating group dimensions
struct RGD {
    uint16_t blockLength; /// Field group size.
    uint8_t numInGroup; /// Number of field groups.
};
#pragma pack(pop)
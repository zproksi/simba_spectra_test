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
static_assert(sizeof(udp_header) == 8, "must be 8");


/// flags for MDPH_ForPacket
enum class MDH_MsgFlags : std::uint16_t {
    LastFragment = 0x1,
    StartOfSnapshot = 0x2,
    EndOfSnapshot = 0x4,
    IncrementalPacket = 0x8,
    PossDupFlag = 0x10
};

///Market Data Packet Header
struct MDPH_ForPacket {
    uint32_t msgSeqNum;
    uint16_t msgSize; // including this header
    uint16_t msgFlags;
    uint64_t sendingTime; //In nanoseconds

    inline
    bool TestFlag(const MDH_MsgFlags toTest) const {
        return (static_cast<uint16_t>(toTest) & msgFlags) == static_cast<uint16_t>(toTest);
    }
};
static_assert(sizeof(MDPH_ForPacket) == 16, "must be 16");

/// SBE Message Header
struct SBE_MessageHeader {
    uint16_t BlockLength;
    uint16_t TemplateID;
    uint16_t SchemaID;
    uint16_t Version;
};
static_assert(sizeof(SBE_MessageHeader) == 8, "must be 8");

/// Incremental Packet Header
struct IncrementalPacketHeader {
    uint64_t TransactTime; ///  time of processing in 'matching'
    uint32_t TradingSessionID; /// Identifier of the trading session
};
static_assert(sizeof(IncrementalPacketHeader) == 12, "must be 12");

struct OrderUpdate_15 {
    int64_t MDEntryID;
    int64_t MDEntryPx;
    int64_t MDEntrySize;
    uint64_t MDFlags;
    uint64_t MDFlags2;
    int32_t SecurityID;
    uint32_t RptSeq;
    uint8_t MDUpdateAction;
    char MDEntryType;
};
static_assert(sizeof(OrderUpdate_15) == 50, "must be 50");


struct OrderExecution_16
{
    int64_t MDEntryID;
    int64_t MDEntryPx;
    int64_t MDEntrySize;
    int64_t LastPx;
    int64_t LastQty;
    int64_t TradeID;
    uint64_t MDFlags;
    uint64_t MDFlags2;
    int32_t SecurityID;
    uint32_t RptSeq;
    uint8_t MDUpdateAction;
    char MDEntryType;
};
static_assert(sizeof(OrderExecution_16) == 74, "must be 74");

#pragma pack(pop)
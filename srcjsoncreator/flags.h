#pragma once

namespace flags
{

enum MDFlagsSet : uint64_t {
    Day = 1ULL << 0,
    IOC = 1ULL << 1,
    NonQuote = 1ULL << 2,
    EndOfTransaction = 1ULL << 12,
    DueToCrossCancel = 1ULL << 13,
    SecondLeg = 1ULL << 14,
    FOK = 1ULL << 19,
    Replace = 1ULL << 20,
    Cancel = 1ULL << 21,
    MassCancel = 1ULL << 22,
    Negotiated = 1ULL << 26,
    MultiLeg = 1ULL << 27,
    CrossTrade = 1ULL << 29,
    NegotiatedMatchByRef = 1ULL << 31,
    COD = 1ULL << 32,
    ActiveSide = 1ULL << 41,
    PassiveSide = 1ULL << 42,
    Synthetic = 1ULL << 45,
    RFS = 1ULL << 46,
    SyntheticPassive = 1ULL << 57,
    BOC = 1ULL << 60,
    DuringDiscreteAuction = 1ULL << 62
};

union NullUnion {
    uint32_t uint32Value;
    char array4[sizeof(uint32_t)];

    NullUnion() {
        array4[0] = 'n';
        array4[1] = 'u';
        array4[2] = 'l';
        array4[3] = 'l';
    }
};

union TrueUnion {
    uint32_t uint32Value;
    char array4[sizeof(uint32_t)];

    TrueUnion() {
        array4[0] = 't';
        array4[1] = 'r';
        array4[2] = 'u';
        array4[3] = 'e';
    }
};
union FalseUnion {
    uint32_t uint32Value;
    char array4[sizeof(uint32_t)];

    FalseUnion() {
        array4[0] = 'f';
        array4[1] = 'a';
        array4[2] = 'l';
        array4[3] = 's';
    }
};



struct MDFlagsSetOUT {
    std::string_view name;
    MDFlagsSet   flag;
};

/// <summary>
///  possible flags for OrderUpdate
/// last name is empty
/// </summary>
extern const struct MDFlagsSetOUT pFlagsOrderUpdate15[];

/// <summary>
///  possible flags for OrderExecution
/// last name is empty
/// </summary>
extern const struct MDFlagsSetOUT pFlagsOrderExecution16[];


}; // namespace flags

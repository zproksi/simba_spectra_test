#include "stdafx.h"
#include <chrono>
#include "formatting.h"
#include "parsing_templates.h"
#include "protocol_structures.h"

namespace formatting {

char* toMemory(std::string_view sv, char* where){
    return std::copy_n(sv.data(), sv.size(), where);
};


/// <summary>
///  print true or false
/// </summary>
char* flagTrueFalse(char* where, const uint64_t v) {
    using namespace flags;
    static TrueUnion tu;
    static FalseUnion fu;
    if (v != 0) {
        *reinterpret_cast<uint32_t*>(where) = tu.uint32Value;
        return where += 4;
    }
    else {
        *reinterpret_cast<uint32_t*>(where) = fu.uint32Value;
        where[4] = 'e';
        return where + 5;
    }

};

/// <summary>
///  prints MDFlags:[...]
/// </summary>
/// <returns>where the poinrter is now</returns>
inline
char* mdflagsPrint(char* where, const uint64_t* apFlags, const flags::MDFlagsSetOUT* const pFlags) {
    where = toMemory(R"("MDFlags":[{)", where);

    where = flagTrueFalse(toMemory(pFlags[0].name, where), *apFlags & pFlags[0].flag);
    *where++ = '}';

    for (size_t i = 1; pFlags[i].name.size() > 0; ++i) {
        *where++ = ',';
        *where++ = '{';
        where = flagTrueFalse(toMemory(pFlags[i].name, where), *apFlags & pFlags[i].flag);
        *where++ = '}';
    }
    *where++ = ']';
    return where;
};




template <int N>
char* fill_the_digits(char* const where, int value)
{
    for (int i = N; i > 0; --i)
    {
        where[i - 1] = '0' + (value % 10);
        value /= 10;
    }
    return where + N;
}

/// <summary>
///  result time will be in "YYYY.MM.DD hh:mm:ss mmm,mmm,nnn" format
///  inside the tosavetheDate. No zero in the end. quotes included
/// </summary>
/// <param name="tosavetheData">where to format the time</param>
/// <param name="utcTime">utc time in nanoseconds</param>
/// <returns> &tosavetheDate[33] </returns>
char* formatTimeUTC(char tosavetheDate[32], const uint64_t* utcTime) {
    using namespace std::chrono;

    // to duration
    nanoseconds ns_since_epoch(*utcTime);
    system_clock::time_point tp(duration_cast<system_clock::duration>(ns_since_epoch));

    std::time_t t_c = system_clock::to_time_t(tp);
    std::tm* const ptm = std::gmtime(&t_c);

    tosavetheDate[0] = '"';
    *fill_the_digits<4>(tosavetheDate + 1, ptm->tm_year + 1900) = '.';
    *fill_the_digits<2>(tosavetheDate + 6, ptm->tm_mon + 1) = '.';
    *fill_the_digits<2>(tosavetheDate + 9, ptm->tm_mday) = ' ';
    *fill_the_digits<2>(tosavetheDate + 12, ptm->tm_hour + 1) = ':';
    *fill_the_digits<2>(tosavetheDate + 15, ptm->tm_min) = ':';
    *fill_the_digits<2>(tosavetheDate + 18, ptm->tm_sec) = ' ';

    const uint64_t fractional = *utcTime  / 1000000000;
    const int iFractional = static_cast<int>(fractional);
    const int iFractionalMilly = iFractional / 1000000;
    const int iFractionalMicro = (iFractional / 1000) % 1000;
    const int iFractionalNano = iFractional % 1000;
    *fill_the_digits<3>(tosavetheDate + 21, iFractionalMilly) = ',';
    *fill_the_digits<3>(tosavetheDate + 25, iFractionalMicro) = ',';
    *fill_the_digits<3>(tosavetheDate + 29, iFractionalNano) = '"';

    return &tosavetheDate[33];
};

char* ForJsonFormatting::FormatOrderUpdate_15(char* pStart, const void* structPointer)
{
    const ForJsonFormatting* pFF = static_cast<const ForJsonFormatting*>(structPointer);

    const OrderUpdate_15* pOrder = reinterpret_cast<const OrderUpdate_15*>(pFF->data_);
    pStart = toMemory(R"({"OrderUpdate":{)", pStart);

    pStart = toJSONObject<int64_t, toValueInt64>("MDEntryID", pStart, &pOrder->MDEntryID);

    *pStart++ = ',';
    pStart = toJSONObject<int64_t, toValueDecimal5>("MDEntryPx", pStart, &pOrder->MDEntryPx);

    *pStart++ = ',';
    pStart = toJSONObject<int64_t, toValueInt64>("MDEntrySize", pStart, &pOrder->MDEntrySize);

    *pStart++ = ',';
    pStart = mdflagsPrint(pStart, &pOrder->MDFlags, flags::pFlagsOrderExecution16);

    //            uint64_t MDFlags2; // skipped - empty

    *pStart++ = ',';
    pStart = toJSONObject<int32_t, toValueInt32>("SecurityID", pStart, &pOrder->SecurityID);

    *pStart++ = ',';
    pStart = toJSONObject<uint32_t, toValueUInt32>("RptSeq", pStart, &pOrder->RptSeq);

    if (0 == pOrder->MDUpdateAction) {
        pStart = toMemory(R"(,"MDUpdateAction":"New")", pStart);
    }
    else if (2 == pOrder->MDUpdateAction) {
        pStart = toMemory(R"(,"MDUpdateAction":"Delete")", pStart);
    }

    if ('0' == pOrder->MDEntryType) {
        pStart = toMemory(R"(,"MDEntryType":"Bid")", pStart);
    }
    else if ('1' == pOrder->MDEntryType) {
        pStart = toMemory(R"(,"MDEntryType":"Ask")", pStart);
    }

    *pStart++ = '}';
    // time of send
    *pStart++ = ',';
    pStart = toJSONObject<uint64_t, formatting::formatTimeUTC>("sendtime", pStart, &pFF->sendingTime_);


    *pStart++ = '}';
    *pStart++ = 0x0d;

    return pStart;
}

char* ForJsonFormatting::FormatOrderExecution_16(char* pStart, const void* structPointer)
{
    const ForJsonFormatting* pFF = static_cast<const ForJsonFormatting*>(structPointer);

    const OrderExecution_16* pOrder = reinterpret_cast<const OrderExecution_16*>(pFF->data_);
    pStart = toMemory(R"({"OrderExecution":{)", pStart);

    pStart = toJSONObject<int64_t, toValueInt64>("MDEntryID", pStart, &pOrder->MDEntryID);

    *pStart++ = ',';
    pStart = toJSONObject<int64_t, toValueDecimal5Null>("MDEntryPx", pStart, &pOrder->MDEntryPx);

    *pStart++ = ',';
    pStart = toJSONObject<int64_t, toValueInt64Null>("MDEntrySize", pStart, &pOrder->MDEntrySize);

    *pStart++ = ',';
    pStart = toJSONObject<int64_t, toValueDecimal5>("LastPx", pStart, &pOrder->LastPx);

    *pStart++ = ',';
    pStart = toJSONObject<int64_t, toValueInt64>("LastQty", pStart, &pOrder->LastQty);

    *pStart++ = ',';
    pStart = toJSONObject<int64_t, toValueInt64>("TradeID", pStart, &pOrder->TradeID);

    *pStart++ = ',';
    pStart = mdflagsPrint(pStart, &pOrder->MDFlags, flags::pFlagsOrderExecution16);

    //            uint64_t MDFlags2; // skipped - empty

    *pStart++ = ',';
    pStart = toJSONObject<int32_t, toValueInt32>("SecurityID", pStart, &pOrder->SecurityID);

    *pStart++ = ',';
    pStart = toJSONObject<uint32_t, toValueUInt32>("RptSeq", pStart, &pOrder->RptSeq);

    if (0 == pOrder->MDUpdateAction) {
        pStart = toMemory(R"(,"MDUpdateAction":"New")", pStart);
    }
    else if (2 == pOrder->MDUpdateAction) {
        pStart = toMemory(R"(,"MDUpdateAction":"Delete")", pStart);
    }

    if ('0' == pOrder->MDEntryType) {
        pStart = toMemory(R"(,"MDEntryType":"Bid")", pStart);
    }
    else if ('1' == pOrder->MDEntryType) {
        pStart = toMemory(R"(,"MDEntryType":"Ask")", pStart);
    }

    *pStart++ = '}';
    // time of send
    *pStart++ = ',';
    pStart = toJSONObject<uint64_t, formatting::formatTimeUTC>("sendtime", pStart, &pFF->sendingTime_);


    *pStart++ = '}';
    *pStart++ = 0x0d;
    return pStart;
}




}; // namespace formatting {





#include "stdafx.h"
#include <chrono>
#include "formatting.h"

namespace formatting {
char* formatTimeUTC(char tosavetheDate[32], const uint64_t utcTime) {
    using namespace std::chrono;

    // to duration
    nanoseconds ns_since_epoch(utcTime);
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

    const uint64_t fractional = utcTime  / 1000000000;
    const int iFractional = static_cast<int>(fractional);
    const int iFractionalMilly = iFractional / 1000000;
    const int iFractionalMicro = (iFractional / 1000) % 1000;
    const int iFractionalNano = iFractional % 1000;
    *fill_the_digits<3>(tosavetheDate + 21, iFractionalMilly) = ',';
    *fill_the_digits<3>(tosavetheDate + 25, iFractionalMicro) = ',';
    *fill_the_digits<3>(tosavetheDate + 29, iFractionalNano) = '"';

    return &tosavetheDate[33];
};

}; // namespace formatting {



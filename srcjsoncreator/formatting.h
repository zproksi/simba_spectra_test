#pragma once

namespace formatting {

template <int N>
char* fill_the_digits(char* const where, int value)
{
    for(int i = N; i > 0; --i)
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
char* formatTimeUTC(char tosavetheDate[32], const uint64_t utcTime);


}; // namespace formatting {

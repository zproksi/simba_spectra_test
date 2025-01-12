#pragma once

#include "flags.h"

// Defining a type alias for the function pointer that matches the `tojson` signature
template<typename T>
using ToJSONFunc = char* (*)(char*, const T*);

template<typename T, ToJSONFunc<T> func>
char* toJSONObject(std::string_view name, char* where, const T* pObj) {

    *where++ = '\"';
    where = std::copy_n(name.data(), name.size(), where);
    *where++ = '\"';
    *where++ = ':';

    // Since `func` is a function pointer, we can directly call it
    return func(where, pObj);
};

constexpr const int64_t NULLINT64 = 9223372036854775807LL;
const char arraytoprint[] = "9876543210123456789";
const char* const pToNumber = &arraytoprint[9];

inline
char* toValueInt64(char* const where, const int64_t* mantissa) {
    // –9223372036854775808 through 9223372036854775807  // 20
    int64_t m = *mantissa;
    const bool negative = m < 0;

    int i = 19;
    do {
        where[i--] = pToNumber[m % ((int64_t)(10))];
        m /= ((int64_t)(10));
    } while (i > 0 && m != 0);

    where[i--] = static_cast<char>(negative) * '-' + static_cast<char>(!negative) * ' ';
    while (i >= 0) { where[i--] = ' '; }
    return where + 20;
}

inline
char* toValueInt64Null(char* const where, const int64_t* mantissa) {
    if (NULLINT64 == *mantissa)
    {
        for (int i = 0; i < 16; ++i) { where[i] = ' '; }
        static flags::NullUnion nu;
        *reinterpret_cast<decltype(nu.uint32Value)*>(&where[16]) = nu.uint32Value;
        return where + 20;
    }
    else
    {
        return toValueInt64(where, mantissa);
    }
}


inline
char* toValueUInt64(char* const where, const uint64_t* mantissa) {
    // 18,446,744,073,709,551,615  // 20
    uint64_t m = *mantissa;

    int i = 20;
    do {
        where[i--] = pToNumber[m % ((uint64_t)(10))];
        m /= ((uint64_t)(10));
    } while (i > 0 && m != 0);

    while (i >= 0) { where[i--] = ' '; }
    return where + 21;
}

inline
char* toValueInt32(char* const where, const int32_t* mantissa) {
    // -2147483648 .. 2147483647 11
    int32_t m = *mantissa;
    const bool negative = m < 0;

    int i = 10;
    do {
        where[i--] = pToNumber[m % ((int32_t)(10))];
        m /= ((int32_t)(10));
    } while (i > 0 && m != 0);

    where[i--] = static_cast<char>(negative) * '-' + static_cast<char>(!negative) * ' ';
    while (i >= 0) { where[i--] = ' '; }
    return where + 11;
}

inline
char* toValueUInt32(char* const where, const uint32_t* mantissa) {
    // 4,294,967,295  // 10
    uint32_t m = *mantissa;

    int i = 9;
    do {
        where[i--] = pToNumber[m % ((uint32_t)(10))];
        m /= ((uint32_t)(10));
    } while (i > 0 && m != 0);

    while (i >= 0) { where[i--] = ' '; }
    return where + 10;
}


inline
char* toValueDecimal5(char* const where, const int64_t* mantissa) {
    // –9223372036854775808 through 9223372036854775807  // 20  &  . => 21
    int64_t m = *mantissa;
    const bool negative = m < 0;

    int i = 20;
    for (; i > 15; --i) {
        where[i] = pToNumber[m % ((int64_t)(10))];
        m /= ((int64_t)(10));
    }
    where[i--] = '.';
    do {
        where[i--] = pToNumber[m % ((int64_t)(10))];
        m /= ((int64_t)(10));
    } while (i > 0 && m != 0);

    where[i--] = static_cast<char>(negative) * '-' + static_cast<char>(!negative) * ' ';
    while (i >= 0) { where[i--] = ' '; }
    return where + 21;
}

inline
char* toValueDecimal5Null(char* const where, const int64_t* mantissa) {
    if (NULLINT64 == *mantissa)
    {
        for (int i = 0; i < 17; ++i) { where[i] = ' '; }
        static flags::NullUnion nu;
        *reinterpret_cast<decltype(nu.uint32Value)*>(&where[17]) = nu.uint32Value;
        return where + 21;
    }
    else
    {
        return toValueDecimal5(where, mantissa);
    }
};

class Decimal5 {
    public:
    explicit Decimal5(const int64_t* mantissa): mantissa_(mantissa) {}
    constexpr static uint64_t maxlength() {return 21;}
    ///  accepts the memory where the value should be printed
    ///  returns the pointer after printed value
    char* toValue(char* const where) {
        // –9223372036854775808 through 9223372036854775807  // 20 & .
        return toValueDecimal5(where, mantissa_);
    }
protected:
    const int64_t* mantissa_;
};

class Decimal5Null {
public:
    explicit Decimal5Null(const int64_t* mantissa): mantissa_(mantissa) {}
    constexpr static uint64_t maxlength() { return 21; }
    ///  accepts the memory where the value should be printed
    ///  returns the pointer after printed value
    char* toValue(char* const where) {
        // –9223372036854775808 through 9223372036854775807  // 20 & .
        return toValueDecimal5Null(where, mantissa_);
    }


protected:
const int64_t* mantissa_;
};


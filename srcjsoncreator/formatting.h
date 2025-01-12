#pragma once

namespace formatting {

/// <summary>
///   pass this structure to the separate thread for formatting
/// </summary>
struct ForJsonFormatting{
    using FORMATTINGFunc = char* (*)(char* input, const void* structPointer);

    uint64_t sendingTime_; // sending time for this struct
    const void* data_; // original data = will be used as const struct
    FORMATTINGFunc func_; // formatter structure

    ForJsonFormatting(const uint64_t sendingTime = 0, const void* data = nullptr, FORMATTINGFunc func = nullptr)
        : sendingTime_(sendingTime)
        , data_(data)
        , func_(func){}

    /// <summary>
    ///  use for initialize func pointer declared above
    /// </summary>
    static char* FormatOrderUpdate_15(char* input, const void* structPointer);

    /// <summary>
    ///  use for initialize func pointer declared above
    /// </summary>
    static char* FormatOrderExecution_16(char* input, const void* structPointer);
};



}; // namespace formatting {

#pragma once

#include <atomic>
#include <cstdlib>
#include <list>
#include <memory>
#include <mutex>
#include <thread>

#include "formatting.h"
#include "tsafequeue.h"


/// <summary>
///    here we hold data for writing to the file
/// </summary>
struct WritingBuffer {
    static const size_t maxBufferCapacity = 4 * 1024 * 1024; // 4 MB
    char data[maxBufferCapacity];
    char* ptrEnd;

    WritingBuffer():ptrEnd(data){}

    /// <returns>Amount of filled data</returns>
    size_t AmountFilled() const noexcept { return (ptrEnd - data); }

    /// need to write buffer
    bool ReadyToWrite() const noexcept
    {
        return AmountFilled() > (maxBufferCapacity * 75 / 100); // 75%  write buffer 
    }

    /// reuse buffer
    char* Reset() noexcept { return ptrEnd = data; }
};


#pragma warning( disable : 4324)
/// <summary>
///   doing write in separate thread
/// </summary>
class
#ifdef __linux__
    __attribute__((aligned(2 * std::hardware_constructive_interference_size)))
#else
    alignas(2 * std::hardware_constructive_interference_size)
#endif
    WritingThread
{
public:
    WritingThread(const char* const filename
        , TSafeQueue<std::unique_ptr<WritingBuffer>>&source // where to peak buffers for writing
        , TSafeQueue<std::unique_ptr<WritingBuffer>>&target);  // where to put buffers after writing
    ~WritingThread();

    void ThreadMain(); // thread will be stopped if get empty WritingBuffer
protected:
    void Write(WritingBuffer* const pBuffer) const;


    FILE* fp_;
    TSafeQueue<std::unique_ptr<WritingBuffer>>& source_;
    TSafeQueue<std::unique_ptr<WritingBuffer>>& target_;
};
static_assert(sizeof(WritingThread) == 2 * std::hardware_constructive_interference_size, "must be this size");


/// <summary>
///   doing formatting in separate thread
/// </summary>
class
#ifdef __linux__
    __attribute__((aligned(2 * std::hardware_constructive_interference_size)))
#else
    alignas(2 * std::hardware_constructive_interference_size)
#endif
FormattingThread
{
public:
    FormattingThread(const char* const filename,
        TSafeQueue<formatting::ForJsonFormatting>& srcToFormat)
    : filename_(filename)
    , srcToFormat_(srcToFormat) {}

    void AddDataToFormat(formatting::ForJsonFormatting&& dta) { srcToFormat_.Add(std::move(dta)); }
    void StopThread();

    void ThreadMain();
protected:
    const char* const filename_;
    TSafeQueue<formatting::ForJsonFormatting>& srcToFormat_;
    TSafeQueue<std::unique_ptr<WritingBuffer>> source_; // where to peak buffers for formatting
    TSafeQueue<std::unique_ptr<WritingBuffer>> target_; // where to put buffers after formatting
    std::unique_ptr<WritingThread> writer_;
};
static_assert(sizeof(FormattingThread) >= 2 * std::hardware_constructive_interference_size, "must be this size");
#pragma warning( default : 4324)


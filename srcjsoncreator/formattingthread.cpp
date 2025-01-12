#include "stdafx.h"
#include "formattingthread.h"


WritingThread::WritingThread(const char* const filename
        , TSafeQueue<std::unique_ptr<WritingBuffer>>& source // where to peak buffers for writing
        , TSafeQueue<std::unique_ptr<WritingBuffer>>& target)  // where to put buffers after writing
    : fp_(std::fopen(filename, "wb"))
    , source_(source)
    , target_(target)
{
}
WritingThread::~WritingThread()
{
    if (fp_) [[likely]] {
        std::fflush(fp_);
        std::fclose(fp_);
        fp_ = nullptr;
    }
}

void WritingThread::Write(WritingBuffer* const pBuffer) const
{
    std::fwrite(pBuffer->data, 1, pBuffer->AmountFilled(), fp_);
}

void WritingThread::ThreadMain()
{
    /// buffer should be with memory to write
    std::unique_ptr<WritingBuffer> buffToWorkWith;
    auto BufferRequest = [&](std::unique_ptr<WritingBuffer>& buf) -> void {
        do {
            buf = source_.Get();
            if (buf == nullptr) {
                _mm_pause();
            }
        } while (buf == nullptr);
    };

    for (BufferRequest(buffToWorkWith); buffToWorkWith->ptrEnd != buffToWorkWith->data; BufferRequest(buffToWorkWith)) {
        Write(buffToWorkWith.get());
        target_.Add(std::move(buffToWorkWith));
    };
};

namespace
{
    constexpr uint64_t forexit = 1;
};

void FormattingThread::StopThread()
{
    srcToFormat_.Add(std::move(formatting::ForJsonFormatting(forexit, nullptr, nullptr)));
}


void FormattingThread::ThreadMain()
{
    /// create 3 buffers for rotation in the beginning
    for (int i = 0; i < 3; ++i) {
        source_.Add(std::move(std::unique_ptr<WritingBuffer>(new WritingBuffer)));
    }

    /// buffer should be with memory to fill
    std::unique_ptr<WritingBuffer> buffToWorkWith;
    auto BufferRequest = [&](std::unique_ptr<WritingBuffer>& buf) -> void {
        do {
            buf = source_.Get();
            if (buf == nullptr) {
                _mm_pause();
            }
        } while (buf == nullptr);
        buf->Reset();
    };

    BufferRequest(buffToWorkWith);
    *buffToWorkWith->ptrEnd++ = '['; // it is array

    /// run writing thread
    writer_.reset(new WritingThread(filename_, target_, source_));
    auto asyncRezult = std::async(std::launch::async, [](void* p) {
        static_cast<WritingThread*>(p)->ThreadMain();
        },
        writer_.get());

    // very first item
    formatting::ForJsonFormatting forfmt = srcToFormat_.Get();
    do {
        if (forfmt.data_ != nullptr)
        {
            buffToWorkWith->ptrEnd = forfmt.func_(buffToWorkWith->ptrEnd, &forfmt);
            break;
        }
        else
        {
            _mm_pause();
        };
    } while (forfmt.sendingTime_ != forexit);

    while (forfmt.sendingTime_ != forexit) {
        forfmt = srcToFormat_.Get();
        if (forfmt.data_ != nullptr)
        {
            *buffToWorkWith->ptrEnd++ = ','; // it is array
            // format data
            buffToWorkWith->ptrEnd = forfmt.func_(buffToWorkWith->ptrEnd, &forfmt);

            // if we have enogh data - send for writing
            if (buffToWorkWith->ReadyToWrite())
            {
                // send buffer for writing
                target_.Add(std::move(buffToWorkWith));

                // get next buffer to fill
                BufferRequest(buffToWorkWith);
            }
        }
        else
        {
            _mm_pause();
        }
    };

    // send last buffer for writing
    *buffToWorkWith->ptrEnd++ = ']'; // close array
    target_.Add(std::move(buffToWorkWith));

    // get next buffer to fill
    BufferRequest(buffToWorkWith);

    // send empty message as a signal to exit
    target_.Add(std::move(buffToWorkWith));

    asyncRezult.get();
};

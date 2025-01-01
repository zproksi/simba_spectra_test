#pragma once
#include <span>

#if !defined(__linux__) && !(defined(__APPLE__) && defined(__MACH__))
// WINDOWS
#include <windows.h>
#endif

/// <summary>
///  Memory Mapped file
/// </summary>
class FileMapper
{
    FileMapper(const FileMapper&) = delete;
    FileMapper(FileMapper&&) = delete;
    FileMapper& operator = (const FileMapper&) = delete;
    FileMapper& operator = (FileMapper&&) = delete;
public:
    FileMapper(const char* const pFileName);
    ~FileMapper();

    std::span<const unsigned char> filedata() const{
        return std::span<unsigned char>(data_, fileSize_);
    }
protected:
#if !defined(__linux__) && !(defined(__APPLE__) && defined(__MACH__))
// WINDOWS
    HANDLE fileHandle_ = 0;
    HANDLE fileMappingHandle_ = 0;

#else
// LINUX MAC
    int fileDescriptor_= -1;
#endif


    std::size_t fileSize_ = 0;
    unsigned char* data_ = nullptr;
};

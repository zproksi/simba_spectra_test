#include "stdafx.h"
#include "filemapper.h"

#if !defined(__linux__) && !(defined(__APPLE__) && defined(__MACH__))
// WINDOWS
FileMapper::FileMapper(const char* const pFileName){
    fileHandle_ = ::CreateFileA(pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle_ == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Failed to open file");

    LARGE_INTEGER size;
    if (!::GetFileSizeEx(fileHandle_, &size))
    {
        ::CloseHandle(fileHandle_);
        throw std::runtime_error("Failed to get file size");
    }
    fileSize_ = static_cast<std::size_t>(size.QuadPart);

    fileMappingHandle_ = ::CreateFileMapping(fileHandle_, NULL, PAGE_READONLY, 0, 0, NULL);
    if (fileMappingHandle_ == NULL)
    {
        ::CloseHandle(fileHandle_);
        throw std::runtime_error("Failed to create file mapping");
    }

    data_ = static_cast<unsigned char*>(MapViewOfFile(fileMappingHandle_, FILE_MAP_READ, 0, 0, 0));
    if (data_ == NULL)
    {
        ::CloseHandle(fileMappingHandle_);
        ::CloseHandle(fileHandle_);
        throw std::runtime_error("Failed to map view of file");
    }
}

FileMapper::~FileMapper(){
    if (data_)
        UnmapViewOfFile(data_);
    if (fileMappingHandle_)
        CloseHandle(fileMappingHandle_);
    if (fileHandle_ != INVALID_HANDLE_VALUE)
        CloseHandle(fileHandle_);
}

#else
// LINUX MAC

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

FileMapper::FileMapper(const char* const pFileName){
    fileDescriptor_ = open(pFileName, O_RDONLY);
    if (fileDescriptor_ == -1)
        throw std::system_error(errno, std::system_category(), "Failed to open file");

    struct stat st;
    if (fstat(fileDescriptor_, &st) == -1) {
        close(fileDescriptor_);
        throw std::system_error(errno, std::system_category(), "Failed to get file size");
    }

    fileSize_ = st.st_size;

    data_ = static_cast<unsigned char*>(mmap(nullptr, fileSize_, PROT_READ, MAP_PRIVATE, fileDescriptor_, 0));
    if (data_ == MAP_FAILED) {
        close(fileDescriptor_);
        throw std::system_error(errno, std::system_category(), "Failed to map file to memory");
    }
}

FileMapper::~FileMapper(){
    if (data_)
        munmap(data_, fileSize_);

    if (fileDescriptor_ != -1)
        close(fileDescriptor_);
}

#endif
#pragma once

#include <string>
#include <unistd.h>


class SafeFile
{
    using string = std::string;

private:
    string path;
    int fd;
public:
    SafeFile();
    // move
    SafeFile(SafeFile&& other) noexcept;
    SafeFile& operator =(SafeFile&& other) noexcept;
    // copy
    SafeFile(const SafeFile&) = delete;
    SafeFile& operator=(const SafeFile&) = delete;
    ~SafeFile();
};

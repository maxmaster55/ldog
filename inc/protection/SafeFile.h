#pragma once

#include <string>
#include <unistd.h>
#include <fcntl.h>

class SafeFile
{
    using string = std::string;

private:
    string path;
    int fd = -1;
public:
    SafeFile(string path);
    void write(std::string& str);
    // move
    SafeFile(SafeFile&& other) noexcept;
    SafeFile& operator =(SafeFile&& other) noexcept;
    // copy
    SafeFile(const SafeFile&) = delete;
    SafeFile& operator=(const SafeFile&) = delete;
    ~SafeFile();
};

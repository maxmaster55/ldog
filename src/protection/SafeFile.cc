#include "./protection/SafeFile.h"


SafeFile::SafeFile(string path): path(std::move(path))
{
    this->path = path;

    fd = open(path.c_str(), O_RDWR);
    if (fd == -1) {
        exit(1); // for now
    }
}


void SafeFile::write(std::string& str)
{
    // ahh name 
    ::write(fd, str.c_str(), str.length());
}

// move
SafeFile::SafeFile(SafeFile&& other) noexcept
    :path(std::move(other.path)), fd(other.fd)
{
    other.fd = -1;
    other.path.clear();
}

SafeFile& SafeFile::operator =(SafeFile&& other)
{
    if (this != &other) {
        // release current resource
        if (fd != -1)
            close(fd);

        // steal resources
        path = std::move(other.path);
        fd = other.fd;

        // invalidate source
        other.fd = -1;
    }
    return *this;
}

SafeFile::~SafeFile()
{
    if (fd != -1) {
        close(fd);
    }
}



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

#include <unistd.h>
#include <errno.h>

bool SafeFile::readLine(std::string& out)
{
    out.clear();

    if (fd == -1)
        return false;

    char ch;
    while (true) {
        ssize_t n = ::read(fd, &ch, 1);

        if (n == 0) {
            // EOF
            return !out.empty();
        }

        if (n == -1) {
            if (errno == EINTR)
                continue;   // retry
            return false;   // real error
        }

        if (ch == '\n') {
            return true;
        }

        out.push_back(ch);
    }
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



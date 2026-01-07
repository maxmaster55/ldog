#include "protection/SafeSocket.h"

SafeSocket::SafeSocket(string _ip, uint16_t _port)
    : ip(std::move(_ip)), port(_port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(1);
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
        std::cerr << "Invalid IP: " << ip << std::endl;
        close(sockfd);
        exit(1);
    }

    if (connect(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect failed");
        close(sockfd);
        exit(1);
    }
}

// send string
bool SafeSocket::sendString(const string& data)
{
    ssize_t total = 0;
    const char* ptr = data.c_str();
    ssize_t len = data.length();

    while (total < len) {
        ssize_t n = ::send(sockfd, ptr + total, len - total, 0);
        if (n <= 0) {
            if (errno == EINTR) continue;
            return false;
        }
        total += n;
    }
    return true;
}

// receive until newline
bool SafeSocket::recvString(string& out)
{
    out.clear();
    char ch;
    while (true) {
        ssize_t n = ::recv(sockfd, &ch, 1, 0);
        if (n == 0) {
            // connection closed
            return !out.empty();
        }
        if (n == -1) {
            if (errno == EINTR) continue;
            return false;
        }
        if (ch == '\n') return true;
        out.push_back(ch);
    }
}

// move constructor
SafeSocket::SafeSocket(SafeSocket&& other) noexcept
    : sockfd(other.sockfd), ip(std::move(other.ip)), port(other.port)
{
    other.sockfd = -1;
}

// move assignment
SafeSocket& SafeSocket::operator=(SafeSocket&& other) noexcept
{
    if (this != &other) {
        if (sockfd != -1) close(sockfd);

        sockfd = other.sockfd;
        ip = std::move(other.ip);
        port = other.port;

        other.sockfd = -1;
    }
    return *this;
}

// destructor
SafeSocket::~SafeSocket()
{
    if (sockfd != -1) {
        close(sockfd);
    }
}

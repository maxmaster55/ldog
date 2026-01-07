#pragma once

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cerrno>
#include <cstring>
#include <iostream>

class SafeSocket
{
    using string = std::string;

private:
    int sockfd = -1;
    string ip;
    uint16_t port;

public:
    SafeSocket(string ip, uint16_t port);
    
    bool sendString(const string& data);
    bool recvString(string& out); // reads until '\n'

    // move
    SafeSocket(SafeSocket&& other) noexcept;
    SafeSocket& operator=(SafeSocket&& other) noexcept;

    // copy
    SafeSocket(const SafeSocket&) = delete;
    SafeSocket& operator=(const SafeSocket&) = delete;

    ~SafeSocket();
};

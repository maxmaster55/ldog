#include <iostream>
#include <chrono>
#include "LogMessage.h"

int main(int argc, char const *argv[])
{
    LogMessage msg("testapp", "ctx", std::chrono::system_clock::now(), LogLevel::DEBUG, "Hello World");

    std::cout << msg;
    return 0;
}

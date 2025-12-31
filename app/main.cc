#include <iostream>
#include <chrono>
#include "LogMessage.h"
#include "ConsoleSinkImpl.h"
#include "FileSinkImpl.h"

int main(int argc, char const *argv[])
{
    LogMessage msg("testapp", "ctx", std::chrono::system_clock::now(), LogLevel::DEBUG, "Hello World");
    LogMessage msg1("hi", "ctx", std::chrono::system_clock::now(), LogLevel::ERROR, "Hello World");
    LogMessage msg2("test", "ctx", std::chrono::system_clock::now(), LogLevel::DEBUG, "Bye World");
    LogMessage msg3("cat", "ctx_cat", std::chrono::system_clock::now(), LogLevel::INFO, "Hello cat");
    FileSinkImpl c("test.txt");

    c.write(msg);
    c.write(msg1);
    c.write(msg2);
    c.write(msg3);
    return 0;
}

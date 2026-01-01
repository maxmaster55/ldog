#include <iostream>
#include <chrono>
#include <memory>
#include "LogMessage.h"
#include "LogManager.h"
#include "ConsoleSinkImpl.h"
#include "FileSinkImpl.h"

int main(int argc, char const *argv[])
{
    LogManager manager;

    manager.add_sink(std::make_unique<FileSinkImpl>("log.txt"));
    manager.add_sink(std::make_unique<ConsoleSinkImpl>());

    LogMessage msg1("hi", "ctx", std::chrono::system_clock::now(), LogLevel::DEBUG, "Hello World, Happy to be alive.");
    LogMessage msg3("cat", "cat_ctx", std::chrono::system_clock::now(), LogLevel::ERROR, "Hello cat, the app is about to crash, please do smth.");
    LogMessage msg2("test", "ctx", std::chrono::system_clock::now(), LogLevel::DEBUG, "Bye World, I'm not about to kill myself, just sleep.");
    
    manager << msg1 << msg2 << msg3;
    manager.write_to_all();

    return 0;
}

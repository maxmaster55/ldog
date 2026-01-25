#include <iostream>
#include <chrono>
#include <memory>
#include "LogMessage.h"
#include "LogManager.h"


int main(int argc, char const *argv[])
{
    LogManager manager(10);


    LogMessage msg1("hi", "ctx", "SeverityLvl", SeverityLvl::WARNING, "Hello World, Happy to be alive.");
    manager << msg1;
    LogMessage msg2("cat", "cat_ctx", "SeverityLvl", SeverityLvl::CRITICAL, "Hello cat, the app is about to crash, please do smth.");
    manager << msg2;
    LogMessage msg3("test", "ctx", "SeverityLvl", SeverityLvl::INFO, "Bye World, I'm not about to kill myself, just sleep.");
    manager << msg3;
    manager << msg3;
    manager << msg3;
    manager << msg3;
    manager << msg3;
    manager << msg3;
    manager << msg3;
    manager << msg3;
    manager << msg3;
    manager << msg3;
    manager << msg3;

    manager.write();

    return 0;
}

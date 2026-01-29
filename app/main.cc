#include <iostream>
#include <chrono>
#include <memory>
#include <sinks/ConsoleSinkImpl.h>
#include <sinks/FileSinkImpl.h>
#include "LogMessage.h"
#include "LogManager.h"
#include <protection/ThreadPool.h>

using namespace std::chrono_literals;

ThreadPool pool(10);

void printer(std::string msg){
    while (true)
    {
        std::cout << msg << "\n";
        std::this_thread::sleep_for(500ms);
    }
}

int main(int argc, char const *argv[])
{
    LogManagerBuilder lm_builder(10);

    LogManager lm = lm_builder.add_sink(std::make_unique<FileSinkImpl>("l1.txt"))
                              .add_sink(std::make_unique<FileSinkImpl>("l1.txt"))
                              .add_sink(std::make_unique<FileSinkImpl>("l2.txt"))
                              .add_sink(std::make_unique<FileSinkImpl>("l3.txt"))
                              .add_sink(std::make_unique<FileSinkImpl>("l4.txt"))
                              .add_sink(std::make_unique<FileSinkImpl>("l5.txt"))
                              .build();

    auto msg = LogMessage("AY 7AGA", "ay 7aga", "now", SeverityLvl::INFO, "hello world");
    lm.add_msg(msg);
    lm.add_msg(msg);
    lm.add_msg(msg);
    lm.add_msg(msg);
    lm.add_msg(msg);

    lm.write();
    return 0;
}

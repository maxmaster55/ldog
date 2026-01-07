#include <iostream>

#include <LogManager.h>
#include <chrono>
#include <stdlib.h>
#include <sinks/ConsoleSinkImpl.h>
#include <sinks/FileSinkImpl.h>
#include <tel_src/FileTelemetrySourceImpl.h>
#include <thread>

int main(int argc, char const *argv[])
{   
    int reads = 3;
    if (argc > 1) reads = atoi(argv[1]);

    
    LogManager manager;
    manager.add_sink(std::make_unique<FileSinkImpl>("log.txt"));
    manager.add_sink(std::make_unique<ConsoleSinkImpl>());


    FileTelemetrySrc fts("/proc/meminfo");

    for (int i = 0; i < reads; i++)
    {
        fts.openSource();
        std::string MemTotal;
        std::string MemFree;
        fts.readSource(MemTotal);
        fts.readSource(MemFree);
        std::string read_str = "read_" + std::to_string(i);

        auto now= std::chrono::system_clock::now();
        LogMessage msg1("memory", read_str, now, LogLevel::INFO, MemTotal);
        LogMessage msg2("memory", read_str, now, LogLevel::INFO, MemFree);
        manager << msg1 << msg2;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        manager.write();
    }
    

    
    return 0;
}

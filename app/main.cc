#include <iostream>
#include <Formatter.h>
#include <LogManager.h>
#include <sinks/FileSinkImpl.h>
#include <sinks/ConsoleSinkImpl.h>
#include <policy/CpuPolicy.h>
#include <policy/GpuPolicy.h>
#include <policy/RamPolicy.h>

int main(int argc, char const *argv[])
{

    LogManager LM = LogManagerBuilder()
    .add_sink(std::make_unique<FileSinkImpl>("tlxxx1.txt"))
    .add_sink(std::make_unique<FileSinkImpl>("tl2.txt"))
    .add_sink(std::make_unique<FileSinkImpl>("tl3.txt"))
    .add_sink(std::make_unique<ConsoleSinkImpl>())
    .build();
    

    LogMessage msg("test", "ctx", "now", SeverityLvl::WARNING, "Bye World, I'm not about to kill myself, just sleep.");
    LM << msg;
    LM.write();
    return 0;
}

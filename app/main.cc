#include <iostream>
#include <chrono>
#include <tel_src/SomeIPTelemetrySourceImpl.h>
#include <protection/ThreadPool.h>
#include <memory>
#include <sinks/ConsoleSinkImpl.h>
#include <sinks/FileSinkImpl.h>
#include "LogMessage.h"
#include "LogManager.h"
#include <Formatter.h>
#include <policy/GpuPolicy.h>

using namespace std::chrono_literals;

ThreadPool pool(10);

int main(int argc, char const *argv[])
{
    Formatter<GpuPolicy> fmt;
    LogManagerBuilder lm_builder(10);

    SomeIPTelemetrySourceImpl &sip = SomeIPTelemetrySourceImpl::instance();
    sip.start();

    LogManager lm = lm_builder.add_sink(std::make_unique<ConsoleSinkImpl>())
                        .add_sink(std::make_unique<FileSinkImpl>("l1.txt"))
                        .add_sink(std::make_unique<FileSinkImpl>("l2.txt"))
                        .add_sink(std::make_unique<FileSinkImpl>("l3.txt"))
                        .add_sink(std::make_unique<FileSinkImpl>("l4.txt"))
                        .add_sink(std::make_unique<FileSinkImpl>("l5.txt"))
                        .build();

    float prev = 0;
    while (true)
    {
        auto val = sip.getLastValue();
        auto numeric_str = std::to_string(val); // convert float to string
        auto msg = fmt.formatDataToLogMsg(numeric_str);
        if (msg && val && prev != val)
        {
            lm.add_msg(msg.value());
            prev = val;
        }
        lm.write();
        std::this_thread::sleep_for(200ms);
    }

    return 0;
}

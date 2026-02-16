#include <iostream>
#include <LogDog.h>
#include <tel_src/SomeIPTelemetrySourceImpl.h>

int main(int argc, char const *argv[])
{
    LogDog logdog("./config.json");

    logdog.start();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
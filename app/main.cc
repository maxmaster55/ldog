#include <iostream>
#include <LogDog.h>
#include <tel_src/SomeIPTelemetrySourceImpl.h>

int main(int argc, char const *argv[])
{
    std::string configPath = "./config.json";
    if (argc > 1)
    {
        configPath = argv[1];
    }
    LogDog logdog(configPath);

    logdog.start();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
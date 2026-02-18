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
    try
    {
        LogDog logdog(configPath);

        logdog.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error initializing LogDog: " << e.what() << std::endl;

        return 1;
    }

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
#include <iostream>
#include <LogDog.h>

int main(int argc, char const *argv[])
{
    LogDog logdog("/home/maxmaster/data/ITI/projects/ldog/config.json");

    logdog.start();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
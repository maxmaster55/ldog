#include "./sinks/ConsoleSinkImpl.h"

void ConsoleSinkImpl::write(const LogMessage& msg)
{
    std::cout << msg; // dman that was easy
}

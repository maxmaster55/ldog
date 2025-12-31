#include "ConsoleSinkImpl.h"

void ConsoleSinkImpl::write(LogMessage& msg)
{
    std::cout << msg; // dman that was easy
}

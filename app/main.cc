#include <iostream>
#include "ConsoleSinkImpl.h"

int main(int argc, char const *argv[])
{
    ConsoleSinkImpl d;
    d.write("test");
    std::cout<<"Hello World\n";
    return 0;
}

#include <iostream>
#include <Formatter.h>
#include <policy/CpuPolicy.h>
#include <policy/GpuPolicy.h>
#include <policy/RamPolicy.h>

int main(int argc, char const *argv[])
{

    Formatter<CpuPolicy> f;
    std::cout << f.formatDataToLogMsg("25.5").value();

    return 0;
}

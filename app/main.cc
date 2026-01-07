#include <iostream>
#include <tel_src/FileTelemetrySourceImpl.h>

int main(int argc, char const *argv[])
{   
    FileTelemetrySrc fts("/home/maxmaster/shell_logs");

    std::string out_str;

    fts.openSource();

    while (fts.readSource(out_str))
        std::cout << out_str << "\n";

    
    return 0;
}

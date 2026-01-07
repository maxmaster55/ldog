#include <iostream>
#include <tel_src/SocketTelemetrySourceImpl.h>

int main(int argc, char const *argv[])
{   
    SocketTelemetrySrc fts("127.0.0.1", 9000);

    std::string out_str;

    fts.openSource();

    while (fts.readSource(out_str))
        std::cout << out_str << "\n";


    
    return 0;
}

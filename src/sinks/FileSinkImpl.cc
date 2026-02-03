#include "./sinks/FileSinkImpl.h"


FileSinkImpl::FileSinkImpl(std::string path)
{

    file = std::ofstream(path);
    if (!file)
    {
        std::cerr << "Can't open file\n";
        exit(1);
    }
    
    
}

void FileSinkImpl::write(const LogMessage& msg)
{
    file << msg; // magic
}
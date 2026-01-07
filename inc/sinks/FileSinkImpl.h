#pragma once
#include "ILogSink.h"
#include "fstream"

class FileSinkImpl: public ILogSink
{
private:
    std::ofstream file;
public:
    FileSinkImpl(std::string path);
    void write(LogMessage& msg);
    ~FileSinkImpl() = default;
};

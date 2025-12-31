#pragma once
#include "ILogSink.h"


class FileSinkImpl: public ILogSink
{
public:
    FileSinkImpl() = default;
    void write(LogMessage& msg);
    ~FileSinkImpl() = default;
};

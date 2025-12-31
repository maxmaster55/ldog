#pragma once
#include "ILogSink.h"


class ConsoleSinkImpl: public ILogSink
{
public:
    ConsoleSinkImpl() = default;
    int write(std::string str);
    ~ConsoleSinkImpl() = default;
};

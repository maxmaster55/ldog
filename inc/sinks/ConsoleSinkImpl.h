#pragma once
#include "ILogSink.h"


class ConsoleSinkImpl: public ILogSink
{
public:
    ConsoleSinkImpl() = default;
    void write(LogMessage& msg) override;
    ~ConsoleSinkImpl() = default;
};

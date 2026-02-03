#pragma once
#include "ILogSink.h"


class ConsoleSinkImpl: public ILogSink
{
public:
    ConsoleSinkImpl() = default;
    void write(const LogMessage& msg) override;
    ~ConsoleSinkImpl() = default;
};

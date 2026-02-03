#pragma once
#include <iostream>
#include <string>
#include "LogMessage.h"


class ILogSink
{
private:

public:
    virtual void write(const LogMessage& msg) = 0;
    virtual ~ILogSink() = default;
};
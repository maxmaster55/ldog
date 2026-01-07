#pragma once

#include <string>

class ITelemetrySource
{
    using string = std::string;
private:

public:
    virtual bool openSource() = 0;
    virtual bool readSource(string& out) = 0;
    ~ITelemetrySource() = default;
};

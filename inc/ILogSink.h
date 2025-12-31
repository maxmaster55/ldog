#include <iostream>
#include <string>



class ILogSink
{
private:

public:
    virtual int write(std::string str) = 0;
    virtual ~ILogSink() = default;
};
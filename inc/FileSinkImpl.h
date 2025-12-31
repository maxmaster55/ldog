#include "ILogSink.h"


class FileSinkImpl: public ILogSink
{
public:
    FileSinkImpl() = default;
    int write(std::string str);
    ~FileSinkImpl() = default;
};

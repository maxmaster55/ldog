#include <tel_src/FileTelemetrySourceImpl.h>


FileTelemetrySourceImpl::FileTelemetrySourceImpl(string path)
: path(std::move(path))
{}


bool FileTelemetrySourceImpl::openSource()
{
    file = SafeFile(path);
    return true;
    
}

bool FileTelemetrySourceImpl::readSource(string& out)
{
    if (!file)
    {
        return false;
    }
    
    return file.value().readLine(out);
    
}
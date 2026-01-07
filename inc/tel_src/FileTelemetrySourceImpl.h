#pragma once

#include "ITelemetrySource.h"
#include "protection/SafeFile.h"

class FileTelemetrySourceImpl: public ITelemetrySource
{
    using string = std::string;
private:
    std::string path;
    std::optional<SafeFile> file;
    
public:
    FileTelemetrySourceImpl(string path);
    bool openSource() override;
    bool readSource(string& out) override;
    ~FileTelemetrySourceImpl() = default;
};

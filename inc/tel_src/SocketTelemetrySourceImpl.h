#pragma onces

#include "ITelemetrySource.h"
#include "protection/SafeSocket.h"


class SocketTelemetrySrc: public ITelemetrySource
{
    using string = std::string;
private:
    string ip;
    uint16_t port;
    std::optional<SafeSocket> sock;
    
public:
    SocketTelemetrySrc(string ip, uint16_t port);
    bool openSource() override;
    bool readSource(string& out) override;
    ~SocketTelemetrySrc() = default;
};

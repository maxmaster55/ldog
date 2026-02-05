#include "tel_src/SomeIPTelemetrySourceImpl.h"
#include <thread>

using namespace std::chrono_literals;

SomeIPTelemetrySrc::SomeIPTelemetrySrc() : lastUsage_(0)
{
}

SomeIPTelemetrySrc &SomeIPTelemetrySrc::instance()
{
    static SomeIPTelemetrySrc inst;
    return inst;
}

void SomeIPTelemetrySrc::start()
{
    auto runtime = CommonAPI::Runtime::get();
    if (!runtime)
    {
        std::cerr << "[Client] ERROR: Failed to get CommonAPI runtime!" << std::endl;
        return;
    }

    std::cout << "using DOMAIN: " << DOMAIN << "\n";
    std::cout << "using INSTANCE: " << INSTANCE << "\n";
        proxy_ = runtime->buildProxy<ldog::gpu::GpuUsageDataProxy>(
        std::string(DOMAIN),
        std::string(INSTANCE));
    if (!proxy_)
    {
        std::cerr << "Failed to create proxy\n";
        return;
    }

    proxy_->getNotifyGpuUsageDataChangeEvent().subscribe(
        [this](const float &usage)
        {
            onGpuUsageCallback(usage);
        });
}

void SomeIPTelemetrySrc::onGpuUsageCallback(float usage)
{
    lastUsage_ = usage;
}

float SomeIPTelemetrySrc::getLastValue() const
{
    return lastUsage_;
}

bool TelemetrySourceAdapter::openSource()
{
    SomeIPTelemetrySrc::instance().start();
    return true;
}

bool TelemetrySourceAdapter::readSource(std::string &out)
{
    out = std::to_string(
        SomeIPTelemetrySrc::instance().getLastValue());
    return true;
}

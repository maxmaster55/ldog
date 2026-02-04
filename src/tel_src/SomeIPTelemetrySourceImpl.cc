#include "tel_src/SomeIPTelemetrySourceImpl.h"
#include <thread>

using namespace std::chrono_literals;

SomeIPTelemetrySourceImpl::SomeIPTelemetrySourceImpl() : lastUsage_(0)
{
}

SomeIPTelemetrySourceImpl &SomeIPTelemetrySourceImpl::instance()
{
    static SomeIPTelemetrySourceImpl inst;
    return inst;
}

void SomeIPTelemetrySourceImpl::start()
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

void SomeIPTelemetrySourceImpl::onGpuUsageCallback(float usage)
{
    lastUsage_ = usage;
}

float SomeIPTelemetrySourceImpl::getLastValue() const
{
    return lastUsage_;
}

bool TelemetrySourceAdapter::openSource()
{
    SomeIPTelemetrySourceImpl::instance().start();
    return true;
}

bool TelemetrySourceAdapter::readSource(std::string &out)
{
    out = std::to_string(
        SomeIPTelemetrySourceImpl::instance().getLastValue());
    return true;
}

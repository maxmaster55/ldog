#pragma once

#include <string>
#include <CommonAPI/CommonAPI.hpp>
#include "ITelemetrySource.h"
#include "v1/ldog/gpu/GpuUsageDataProxy.hpp"

using namespace v1;

constexpr std::string_view DOMAIN = "local";
constexpr std::string_view INSTANCE = "ldog.gpu.GpuUsageData";

class SomeIPTelemetrySourceImpl
{

private:
    std::shared_ptr<ldog::gpu::GpuUsageDataProxy<>> proxy_;
    float lastUsage_{0.0f};
    SomeIPTelemetrySourceImpl();
    void onGpuUsageCallback(float usage);

public:
    static SomeIPTelemetrySourceImpl &instance();
    void start();
    float getLastValue() const;
    // no copy and move
    SomeIPTelemetrySourceImpl(const SomeIPTelemetrySourceImpl &) = delete;
    SomeIPTelemetrySourceImpl &operator=(const SomeIPTelemetrySourceImpl &) = delete;
    SomeIPTelemetrySourceImpl(SomeIPTelemetrySourceImpl &&) = delete;
    SomeIPTelemetrySourceImpl &operator=(SomeIPTelemetrySourceImpl &&) = delete;
};

class TelemetrySourceAdapter : public ITelemetrySource
{
public:
    bool openSource();
    bool readSource(std::string &out);
};

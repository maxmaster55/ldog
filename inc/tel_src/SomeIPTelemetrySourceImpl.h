#pragma once

#include <string>
#include <CommonAPI/CommonAPI.hpp>
#include "ITelemetrySource.h"
#include "v1/ldog/gpu/GpuUsageDataProxy.hpp"

using namespace v1;

constexpr std::string_view DOMAIN = "local";
constexpr std::string_view INSTANCE = "ldog.gpu.GpuUsageData";

class SomeIPTelemetrySrc
{

private:
    std::shared_ptr<ldog::gpu::GpuUsageDataProxy<>> proxy_;
    float lastUsage_{0.0f};
    SomeIPTelemetrySrc();
    void onGpuUsageCallback(float usage);

public:
    static SomeIPTelemetrySrc &instance();
    void start();
    float getLastValue() const;
    // no copy and move
    SomeIPTelemetrySrc(const SomeIPTelemetrySrc &) = delete;
    SomeIPTelemetrySrc &operator=(const SomeIPTelemetrySrc &) = delete;
    SomeIPTelemetrySrc(SomeIPTelemetrySrc &&) = delete;
    SomeIPTelemetrySrc &operator=(SomeIPTelemetrySrc &&) = delete;
};

class TelemetrySourceAdapter : public ITelemetrySource
{
public:
    bool openSource();
    bool readSource(std::string &out);
};

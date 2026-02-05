#pragma once

#include <json/json.hpp>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <fstream>
#include <optional>

#include <magic_enum/magic_enum.hpp>

#include <tel_src/SomeIPTelemetrySourceImpl.h>
#include <tel_src/FileTelemetrySourceImpl.h>
#include <tel_src/SocketTelemetrySourceImpl.h>

#include <sinks/ConsoleSinkImpl.h>
#include <sinks/FileSinkImpl.h>

#include <policy/CpuPolicy.h>
#include <policy/GpuPolicy.h>
#include <policy/RamPolicy.h>

#include "LogManager.h"
#include "LogMessage.h"
#include <Formatter.h>

using json = nlohmann::json;

using CpuFormatter = Formatter<CpuPolicy>;
using GpuFormatter = Formatter<GpuPolicy>;
using RamFormatter = Formatter<RamPolicy>;

struct TelemetryEntry
{
    std::unique_ptr<ITelemetrySource> src;
    TelemetrySrc policy_sel;
    int32_t updateRateMs;
    std::string lastValue;
};

struct SinkEntry
{
    LogSinkType sink;
    std::string name;
    std::optional<std::string> path;
};

class LogDog
{
private:
    LogManager lm_;
    ThreadPool app_pool_;

    std::vector<TelemetryEntry>
        telemetryEntries;
    std::vector<SinkEntry> sinkEntries;

    int logRateMs = 200;

    /* parsing */
    TelemetrySrc parsePolicy(const std::string &s);
    std::optional<LogMessage> formatWithPolicy(TelemetrySrc policy, const std::string &raw) const;
    std::unique_ptr<ITelemetrySource> makeTelemetrySource(const json &t);
    void loadConfig(const std::string &configPath);

    /* setup */
    // void setupTelemetry();
    void setupLogging();

public:
    explicit LogDog(const std::string &configPath, int msg_buffer_size = 1000);
    void start();
};

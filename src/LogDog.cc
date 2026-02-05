#include "LogDog.h"

TelemetrySrc LogDog::parsePolicy(const std::string &s)
{
    auto val = magic_enum::enum_cast<TelemetrySrc>(s);
    if (!val)
        throw std::runtime_error("Unknown policy: " + s);
    return *val;
}

std::unique_ptr<ITelemetrySource>
LogDog::makeTelemetrySource(const json &t)
{
    const std::string source = t.at("source").get<std::string>();

    if (source == "SOMEIP")
    {
        auto adapter = std::make_unique<TelemetrySourceAdapter>();
        return adapter;
    }
    else if (source == "FILE")
    {
        if (!t.contains("path"))
            throw std::runtime_error("File telemetry requires 'path'");

        auto file = std::make_unique<FileTelemetrySrc>(
            t.at("path").get<std::string>());
        return file;
    }
    else if (source == "SOCKET")
    {
        if (!t.contains("ip") || !t.contains("port"))
            throw std::runtime_error("Socket telemetry requires 'ip' and 'port'");

        auto sock = std::make_unique<SocketTelemetrySrc>(
            t.at("ip").get<std::string>(),
            t.at("port").get<uint16_t>());

        return sock;
    }

    throw std::runtime_error("Unknown telemetry source: " + source);
}

void LogDog::loadConfig(const std::string &configPath)
{
    std::ifstream f(configPath);
    if (!f.is_open())
        throw std::runtime_error("Failed to open config: " + configPath);

    json cfg;
    f >> cfg;

    if (cfg.contains("logging"))
        logRateMs = cfg["logging"].value("rate", logRateMs);

    if (!cfg.contains("telemetry") || !cfg["telemetry"].is_array())
        throw std::runtime_error("telemetry must be an array");

    for (const auto &t : cfg["telemetry"])
    {
        TelemetryEntry e{};

        e.updateRateMs =
            t.at("updateIntervalMs").get<int32_t>();

        e.policy_sel =
            parsePolicy(t.at("policy").get<std::string>());

        e.src =
            makeTelemetrySource(t);

        telemetryEntries.emplace_back(std::move(e));
    }

    if (!cfg.contains("sinks") || !cfg["sinks"].is_array())
        throw std::runtime_error("sinks must be an array");

    for (const auto &s : cfg["sinks"])
    {
        SinkEntry e{};

        if (!s.contains("type"))
            throw std::runtime_error("Sink entry must have 'type'");

        auto sinkTypeOpt = magic_enum::enum_cast<LogSinkType>(s.at("type").get<std::string>());
        if (!sinkTypeOpt)
            throw std::runtime_error("Unknown sink type: " + s.at("type").get<std::string>());

        e.sink = *sinkTypeOpt;

        if (s.contains("path"))
            e.path = s.at("path").get<std::string>();

        sinkEntries.emplace_back(std::move(e));
    }
}

std::optional<LogMessage> LogDog::formatWithPolicy(TelemetrySrc policy, const std::string &raw) const
{
    switch (policy)
    {
    case TelemetrySrc::CPU:
    {
        static CpuFormatter fmt;
        return fmt.formatDataToLogMsg(raw);
    }
    case TelemetrySrc::GPU:
    {
        static GpuFormatter fmt;
        return fmt.formatDataToLogMsg(raw);
    }
    case TelemetrySrc::RAM:
    {
        static RamFormatter fmt;
        return fmt.formatDataToLogMsg(raw);
    }
    }

    return std::nullopt; // unreachable, but keeps compiler happy
}

void LogDog::setupLogging()
{
    for (auto sink : sinkEntries)
    {
        switch (sink.sink)
        {
        case LogSinkType::CONSOLE:
            lm_.add_sink(std::make_unique<ConsoleSinkImpl>());
            break;
        case LogSinkType::FILE:
            if (!sink.path)
                throw std::runtime_error("File sink requires 'path'");
            lm_.add_sink(std::make_unique<FileSinkImpl>(*sink.path));
            break;
        case LogSinkType::SOCKET:
            throw std::runtime_error("Socket sink not implemented");
            break;
        default:
            throw std::runtime_error("Unknown sink type: " + std::to_string((int)sink.sink));
            break;
        }
    }
}

LogDog::LogDog(const std::string &configPath, int msg_buffer_size) : lm_(msg_buffer_size)
{
    loadConfig(configPath);
}

void LogDog::start()
{
    setupLogging();

    for (auto &entry : telemetryEntries)
    {
        entry.src->openSource();
        app_pool_.enqueue([this, &entry]()
                          {
            while (true)
            {
                std::string raw;
                if (entry.src->readSource(raw) && raw != entry.lastValue && !raw.empty())
                {
                    auto logMsgOpt = formatWithPolicy(entry.policy_sel, raw);
                    if (logMsgOpt)
                    {
                        lm_ << *logMsgOpt;
                    }
                    entry.lastValue = raw;
                }
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(entry.updateRateMs));
            } });
    }

    app_pool_.enqueue([this]()
                      {
        while (true)
        {
            lm_.write();
            std::this_thread::sleep_for(
                std::chrono::milliseconds(logRateMs));
        } });
}

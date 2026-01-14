#pragma once
#include <log_types.h>


struct GpuPolicy 
{
    static constexpr TelemetrySrc context = TelemetrySrc::GPU;
    static constexpr std::string_view unit = "%";
    static constexpr float WARNING = 60.0f;
    static constexpr float CRITICAL = 85.0f;

    static constexpr SeverityLvl inferSeverity(float val) noexcept {
        return (val > CRITICAL) ? SeverityLvl::CRITICAL
            : (val > WARNING)  ? SeverityLvl::WARNING
            :                    SeverityLvl::INFO;
    }
};
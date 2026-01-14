#pragma once
#include <log_types.h>


struct CpuPolicy 
{
    static constexpr TelemetrySrc context = TelemetrySrc::CPU;
    static constexpr std::string_view unit = "%";
    static constexpr float WARNING = 75.0f;
    static constexpr float CRITICAL = 90.0f;

    static constexpr SeverityLvl inferSeverity(float val) noexcept {
        return (val > CRITICAL) ? SeverityLvl::CRITICAL
            : (val > WARNING)  ? SeverityLvl::WARNING
            :                    SeverityLvl::INFO;
    }
};
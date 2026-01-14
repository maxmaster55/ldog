#pragma once
#include <log_types.h>


struct RamPolicy 
{
    static constexpr TelemetrySrc context = TelemetrySrc::RAM;
    static constexpr std::string_view unit = "%";
    static constexpr float WARNING = 70.0f;
    static constexpr float CRITICAL = 85.0f;

    static constexpr SeverityLvl inferSeverity(float val) noexcept {
        return (val > CRITICAL) ? SeverityLvl::CRITICAL
            : (val > WARNING)  ? SeverityLvl::WARNING
            :                    SeverityLvl::INFO;
    }
};
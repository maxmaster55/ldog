#pragma once

#include <magic_enum/magic_enum.hpp>

enum class LogSinkType {
    CONSOLE,
    FILE,
    SOCKET
};

enum class SeverityLvl {
    CRITICAL,
    WARNING,
    INFO
};


enum class TelemetrySrc {
    CPU,
    GPU,
    RAM
};
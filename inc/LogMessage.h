#pragma once
#include <log_types.h>
#include <string>
#include <chrono>



class LogMessage
{
    using clock = std::chrono::system_clock;
    using time_point = clock::time_point;
    using string = std::string;
public:
    string app_name;
    string context;
    time_point time;
    SeverityLvl level;
    string text;

    LogMessage(string app_name, string context, time_point time, SeverityLvl level, string text);
    ~LogMessage() = default;
};


std::ostream& operator<<(std::ostream& os, const LogMessage& msg);
#include "LogMessage.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>


std::string tp_to_string(const std::chrono::system_clock::time_point& tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&t); // convert to local time
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const LogMessage& msg) {
    os <<"[" << msg.app_name << "] "
    <<"[" << tp_to_string(msg.time) << "] " 
    <<"[" << msg.context << "] "
    <<"[" << magic_enum::enum_name(msg.level) << "] "
    <<"[" << msg.text << "]"
    << std::endl;
    return os;
}



LogMessage::LogMessage(string app_name, string context, time_point time, SeverityLvl level, string text)
:app_name(app_name), context(context), time(time), level(level), text(text)
{
    
}

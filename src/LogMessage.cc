#include "LogMessage.h"

#include <iostream>



std::ostream& operator<<(std::ostream& os, const LogMessage& msg) {
    os <<"[" << msg.app_name << "] "
    <<"[" << msg.time << "] " 
    <<"[" << msg.context << "] "
    <<"[" << magic_enum::enum_name(msg.level) << "] "
    <<"[" << msg.text << "]"
    << std::endl;
    return os;
}



LogMessage::LogMessage(string app_name, string context, string time, SeverityLvl level, string text)
:app_name(app_name), context(context), time(time), level(level), text(text)
{
    
}

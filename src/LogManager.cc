#include "LogManager.h"


void LogManager::add_sink(unique_ptr<ILogSink> sink)
{
    sinks.push_back(std::move(sink));
}

void LogManager::write()
{
    for (auto& sink : sinks){
        for (auto& msg : messages)
        {
            sink->write(msg);
        }
    }

    messages.clear(); // empty the buffer
}


void LogManager::add_msg(LogMessage& msg){
    messages.push_back(msg);
}

LogManager& LogManager::operator<<(LogMessage& msg){
    this->add_msg(msg);
    return *this;
}

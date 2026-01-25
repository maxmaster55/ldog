#include "LogManager.h"


void LogManager::add_sink(unique_ptr<ILogSink> sink)
{
    sinks.push_back(std::move(sink));
}

void LogManager::write()
{
    int msgs_size = messages.size();
    for (auto& sink : sinks){
        for (int i = 0; i < msgs_size; i++){
            auto msg = messages.tryPop();
            if (msg)
            {
                sink->write(msg.value());
            }
            
        }
    }
}


void LogManager::add_msg(LogMessage& msg){
    if (!messages.tryPush(msg))
    {
        std::cout << "error, full\n";
    }
}

LogManager& LogManager::operator<<(LogMessage& msg){
    this->add_msg(msg);
    return *this;
}

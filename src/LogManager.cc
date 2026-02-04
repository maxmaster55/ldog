#include "LogManager.h"

void LogManager::add_sink(unique_ptr<ILogSink> sink)
{
    sinks.push_back(std::move(sink));
}

void LogManager::write()
{
    std::vector<LogMessage> msgs;
    msgs.reserve(messages.size());

    while (auto msg = messages.tryPop())
    {
        std::cout << msg.value();
        msgs.push_back(std::move(*msg));
    }

    for (auto &sink : sinks)
    {
        pool_->enqueue([&sink, msgs]()
                       {
            for (const LogMessage& msg : msgs)
                sink->write(msg); });
    }
}

void LogManager::add_msg(LogMessage &msg)
{
    if (!messages.tryPush(msg))
    {
        std::cout << "error, full\n";
    }
}

LogManager &LogManager::operator<<(LogMessage &msg)
{
    this->add_msg(msg);
    return *this;
}

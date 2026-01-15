#pragma once
#include <vector>
#include <memory>
#include "LogMessage.h"
#include "./sinks/ILogSink.h"

/* LogManager → a class that for now stores LogMessage objects into a private buffer
 *              and has a method that routes the messages into all sinks.
*/
using std::unique_ptr;

class LogManager
{
private:
    std::vector<LogMessage> messages;
    std::vector<unique_ptr<ILogSink>> sinks;

public:
    LogManager() = default;
    void add_msg(LogMessage& msg);
    LogManager& operator<<(LogMessage& msg);
    void add_sink(unique_ptr<ILogSink> sink);
    void write();
    // copy
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;
    // move
    LogManager(LogManager&& other) noexcept = default;
    LogManager& operator=(LogManager&& other) noexcept = default;
    ~LogManager() = default;
};


class LogManagerBuilder{
private:  
    LogManager manager;
public:
    LogManagerBuilder& add_sink(unique_ptr<ILogSink> sink)
    {
        manager.add_sink(std::move(sink));
        return *this;
    }
    LogManagerBuilder& add_msg(LogMessage& msg){
        manager.add_msg(msg);
        return *this;
    }
    LogManager build(){
        return std::move(manager);
    }
    
    ~LogManagerBuilder() = default;
};
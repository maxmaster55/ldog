#pragma once
#include <vector>
#include <memory>
#include <protection/RingBuffer.h>
#include <protection/ThreadPool.h>
#include "LogMessage.h"
#include "./sinks/ILogSink.h"

/* LogManager → a class that for now stores LogMessage objects into a private buffer
 *              and has a method that routes the messages into all sinks.
*/
using std::unique_ptr;

class LogManager
{
private:
    RingBuffer<LogMessage> messages;
    std::vector<unique_ptr<ILogSink>> sinks;
    std::unique_ptr<ThreadPool> pool_;

public:
    LogManager(
        int capacity,
        std::unique_ptr<ThreadPool> pool = std::make_unique<ThreadPool>(24)
    ) : messages(capacity), pool_(std::move(pool)) { }

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
    LogManagerBuilder(int capacity): manager(capacity) {}

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
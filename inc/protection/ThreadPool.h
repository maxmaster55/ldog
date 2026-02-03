#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

class ThreadPool {
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_;

    void worker_loop()
    {
        while (true) {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv_.wait(lock, [this] {
                    return stop_ || !tasks_.empty();
                });

                if (stop_ && tasks_.empty())
                    return;

                task = std::move(tasks_.front());
                tasks_.pop();
            }

            // Execute outside the lock
            task();
        }
    }

public:
    explicit ThreadPool(std::size_t thread_count = 24)
        : stop_(false)
    {
        if (thread_count == 0)
            throw std::invalid_argument("ThreadPool size must be > 0");

        for (std::size_t i = 0; i < thread_count; ++i) {
            workers_.emplace_back([this] {
                worker_loop();
            });
        }
    }

    // No copy 
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // Push a task into the queue
    void enqueue(std::function<void()> task)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stop_)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks_.push(std::move(task));
        }
        cv_.notify_one();
    }

    ~ThreadPool()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop_ = true;
        }

        cv_.notify_all();

        for (auto &t : workers_) {
            if (t.joinable())
                t.join();
        }
    }

};

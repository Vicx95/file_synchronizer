#pragma once

#include "logger.hpp"
#include "thread_pool.hpp"

#include <atomic>
#include <memory>

class ThreadPoolProvider
{

public:
    static ThreadPoolProvider &instance()
    {
        static ThreadPoolProvider s; // initialized the first time control passes to this declaration. Ignored on subsequent calls.
                                     // Since C++11, If multiple threads attempt to initialize the same static local variable concurrently,
                                     // the initialization is still guaranteed to occur exactly once.
                                     // see https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables
        return s;
    }

    ThreadPool *getThreadPool()
    {
        if (m_tp)
        {
            return m_tp.get();
        }
        else
        {
            throw(std::logic_error("ThreadPool is nullptr!"));
        }
    }

private:
    ThreadPoolProvider()
    {
        m_tp = std::make_unique<ThreadPool>();
        LOG_INFO("Thread Pool Provider instantiated");
    }

    ~ThreadPoolProvider() = default;
    ThreadPoolProvider(const ThreadPoolProvider &) = delete;
    ThreadPoolProvider &operator=(const ThreadPoolProvider &) = delete;

    std::unique_ptr<ThreadPool> m_tp;
};
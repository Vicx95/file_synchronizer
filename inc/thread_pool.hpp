#pragma once

#include <algorithm>
#include <atomic>
#include <concepts>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

class Base
{
public:
    virtual ~Base() = default;
    virtual void operator()() = 0;
};

template <typename F>
class Impl : public Base
{
public:
    explicit Impl(F &&fun)
        : m_fun(std::move(fun))
    {
    }

    void operator()() override
    {
        std::invoke(m_fun);
    }

private:
    F m_fun;
};

class MoveOnlyTask
{
public:
    template <typename F>
    MoveOnlyTask(F &&fun)
        : m_impl(std::make_unique<Impl<F>>(std::move(fun)))
    {
    }

    MoveOnlyTask() = default;
    ~MoveOnlyTask() = default;
    MoveOnlyTask(const MoveOnlyTask &) = delete;
    MoveOnlyTask &operator=(const MoveOnlyTask &) = delete;
    MoveOnlyTask(MoveOnlyTask &&) noexcept = default;
    MoveOnlyTask &operator=(MoveOnlyTask &&) noexcept = default;

    void operator()() const
    {
        if (!m_impl)
        {
            throw std::runtime_error("No function to call!");
        }
        std::invoke(*m_impl);
    }

private:
    std::unique_ptr<Base> m_impl = nullptr;
};

class ThreadPool
{
public:
    ThreadPool()
    {
        const size_t thread_count = std::max(4u, std::thread::hardware_concurrency());
        m_threads.reserve(thread_count);

        for (size_t i = 0; i < thread_count; ++i)
        {
            m_threads.emplace_back(&ThreadPool::worker, this);
        }
    }

    ~ThreadPool()
    {
        m_running = false;
        for (auto &t : m_threads)
        {
            t.join();
        }
    }

    auto submit(std::invocable auto &&fun)
    {
        std::packaged_task<std::invoke_result_t<decltype(fun)>()> task(fun);
        std::future future = task.get_future();

        std::lock_guard lock(m_mutex);
        m_queue.push(std::move(task));

        return future;
    }

private:
    void worker()
    {
        while (m_running)
        {
            MoveOnlyTask task;
            if (pop_task(task))
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

    bool pop_task(MoveOnlyTask &task)
    {
        std::lock_guard lock(m_mutex);

        if (m_queue.empty())
        {
            return false;
        }
        task = std::move(m_queue.front());
        m_queue.pop();

        return true;
    }

    std::atomic<bool> m_running = true;
    std::mutex m_mutex;
    std::queue<MoveOnlyTask> m_queue;
    std::vector<std::thread> m_threads;
};
/* #include "..//inc/ThreadPool.hpp"

auto ThreadPool::submit(std::invocable auto &&fun)
{
    std::packaged_task<std::invoke_result_t<decltype(fun)>()> task(fun);
    std::future future = task.get_future();

    std::lock_guard lock(m_mutex);
    m_queue.push(std::move(task));

    return future;
}

void ThreadPool::worker()
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

bool ThreadPool::pop_task(MoveOnlyTask &task)
{
    std::lock_guard lock(m_mutex);

    if (m_queue.empty())
    {
        return false;
    }
    task = std::move(m_queue.front());
    m_queue.pop();

    return true;
} */
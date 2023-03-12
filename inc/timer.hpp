#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

typedef std::chrono::milliseconds Interval; // do zmiany na minuty/sekundy
typedef std::function<void(void)> Callback;

class i_Timer
{
public:
    virtual void start(const Interval &interval, const Callback &timeoutCallback) = 0;
    virtual void stop() = 0;
};

class Timer : public i_Timer
{
public:
    Timer();
    virtual ~Timer();

    Timer(const Timer &) = default;
    Timer &operator=(const Timer &) = default;
    Timer(Timer &&) = default;
    Timer &operator=(Timer &&) = default;

    void start(const Interval &interval, const Callback &timeoutCallback) override;
    void stop() override;

private:
    std::thread m_thread{};
    std::atomic_bool m_timerIsRunning{};
};

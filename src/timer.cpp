#include "..//inc/timer.hpp"

Timer::~Timer()
{
    if (m_timerIsRunning)
    {
        stop();
    }
}

void Timer::start(const Interval &interval, const Callback &timeoutCallback)
{
    m_timerIsRunning = true;

    m_thread = std::thread([this, interval, timeoutCallback] {
        while (m_timerIsRunning)
        {
            std::this_thread::sleep_for(interval);
            timeoutCallback();
        }
    });
}

void Timer::stop()
{
    m_timerIsRunning = false;
    m_thread.join();
}
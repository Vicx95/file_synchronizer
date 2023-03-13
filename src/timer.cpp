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
    if (!m_timerIsRunning)
    {
        m_timerIsRunning = true;
        std::cout << "Timer started";

        m_thread = std::thread([this, interval, timeoutCallback] {
            while (m_timerIsRunning)
            {
                std::this_thread::sleep_for(interval);
                timeoutCallback();
            }
        });
    }
}

void Timer::stop()
{
    m_timerIsRunning = false;

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}
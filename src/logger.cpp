#include "inc/logger.hpp"

#include <iostream>

std::string I_Logger::makeTimestamp()
{
    auto timePoint = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(timePoint);
    char buf[20];
    strftime(buf, 20, "%d.%m.%Y %H:%M:%S", localtime(&t));
    std::string s(buf);
    return s;
}

void I_Logger::prepareMessage(const std::string &message, const LogLevel level)
{
    if (level < LOG_LEVEL_CUTOFF)
        return;
    std::string output;
    output.reserve(message.length() + 64);
    output.append(makeTimestamp());
    output.append(uncolored.find(level)->second);
    output.append(message);
    output.push_back('\n');
    logMessage(output);
}

void I_Logger::logMessage(const std::string &message)
{
    std::unique_lock lock(m_queueMutex);
    m_messageQueue.push(message);
    m_condVar.notify_one();
}

FileLogger::FileLogger()
{
    m_loggingThread = std::thread{&FileLogger::processEntries, this};
}

FileLogger::~FileLogger()
{
    m_exit = true;

    m_condVar.notify_one();
    m_loggingThread.join();
}

void FileLogger::processEntries()
{
    std::ofstream logFile(path);
    if (logFile.fail())
    {
        std::cerr << "Failed to open logfile." << std::endl;
        return;
    }

    while (!m_exit)
    {
        std::unique_lock lock(m_queueMutex);
        m_condVar.wait(lock, [this] { return (!m_messageQueue.empty() or m_exit); });

        if (!m_messageQueue.empty())
        {
            logFile << m_messageQueue.front() << std::endl;
            m_messageQueue.pop();
        }
    }
}
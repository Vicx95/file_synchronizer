#pragma once

#include <atomic>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <unordered_map>

enum class LogLevel : uint8_t
{
    DEBUG = 0,
    INFO,
    WARN,
    ERROR
};

struct enum_hasher
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};
const std::unordered_map<LogLevel, std::string, enum_hasher> uncolored
{
    {LogLevel::ERROR, " [ERROR] "},
    {LogLevel::WARN, " [WARN] "},
    {LogLevel::INFO, " [INFO] "},
    {LogLevel::DEBUG, " [DEBUG] "},
};

const std::unordered_map<LogLevel, std::string, enum_hasher> colored
{
    {LogLevel::ERROR, " \x1b[31;1m[ERROR]\x1b[0m "},
    {LogLevel::WARN, " \x1b[33;1m[WARN]\x1b[0m "},
    {LogLevel::INFO, " \x1b[32;1m[INFO]\x1b[0m "},
    {LogLevel::DEBUG, " \x1b[34;1m[DEBUG]\x1b[0m "},
};

#if defined(LOGGING_LEVEL_ALL)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::DEBUG;
#elif defined(LOGGING_LEVEL_DEBUG)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::DEBUG;
#elif defined(LOGGING_LEVEL_WARN)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::WARN;
#elif defined(LOGGING_LEVEL_ERROR)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::ERROR;
#elif defined(LOGGING_LEVEL_NONE)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::ERROR + 1;
#else
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::INFO;
#endif

using logging_config_t = std::unordered_map<std::string, std::string>;
class I_Logger
{
public:
    I_Logger() = default;
    virtual ~I_Logger() = default;
    I_Logger(const I_Logger &) = delete;
    I_Logger &operator=(const I_Logger &) = delete;
    I_Logger(I_Logger&&) = delete;
    I_Logger &operator=(I_Logger&&) = delete;
    virtual void prepareMessage(const std::string &, const LogLevel);

protected:
    std::mutex m_queueMutex;
    std::condition_variable m_condVar;
    std::queue<std::string> m_messageQueue;
    std::thread m_loggingThread;
    std::atomic_bool m_exit = false;
    virtual void logMessage(const std::string &);
    virtual std::string makeTimestamp();

private:    
    virtual void processEntries() = 0;
};

class FileLogger : public I_Logger
{
public:
    FileLogger(const FileLogger &) = delete;
    FileLogger &operator=(const FileLogger &) = delete;
    FileLogger(FileLogger &&) = delete;
    FileLogger &operator=(FileLogger &&) = delete;
    static FileLogger &getLogger()
    {
        static FileLogger logger;
        return logger;
    }

private:
    const char *path = "./log.txt";
    FileLogger();
    ~FileLogger();
    void processEntries() override;
};

inline void LOG_DEBUG(const std::string &message)
{
    FileLogger::getLogger().prepareMessage(message, LogLevel::DEBUG);
}
inline void LOG_INFO(const std::string &message)
{
    FileLogger::getLogger().prepareMessage(message, LogLevel::INFO);
}
inline void LOG_WARN(const std::string &message)
{
    FileLogger::getLogger().prepareMessage(message, LogLevel::WARN);
}
inline void LOG_ERROR(const std::string &message)
{
    FileLogger::getLogger().prepareMessage(message, LogLevel::ERROR);
}
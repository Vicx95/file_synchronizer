#pragma once

#include "FileSynchronizer.hpp"
#include "timer.hpp"
#include "Serializer.hpp"
#include <filesystem>

namespace fs = std::filesystem;

enum class ErrorCode
{
    SUCCESS = 0,
    FAIL = 1
};

class Model
{
public:
    Model(i_Timer *syncTimer, i_FileSynchronizer *fileSynchronizer, i_Scanner *scanner);

    ErrorCode addDirectory(std::istream &std_input);
    ErrorCode removeDirectory(std::istream &std_input);
    ErrorCode removeFile();

    void setIntervalTime(std::istream &std_input);
    void startSync();
    void stopSync();
    void forceSync();
    void readConfig(std::istream &std_input);
    void saveConfig();
    void addConfig(std::istream &std_input);
    fs::path getMainDirectoryPath();
    bool m_syncStarted = false;

private:
    bool validateForRemoval(std::string name);

    i_Timer *m_syncTimer;
    i_FileSynchronizer *m_fileSynchronizer;
    i_Scanner *m_scanner;
    std::unique_ptr<Serializer> m_serializer;

    std::chrono::duration<int64_t, std::milli> m_interval;
    const fs::path m_mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";
};

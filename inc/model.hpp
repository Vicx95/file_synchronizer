#pragma once

#include "file_synchronizer.hpp"
#include "serializer.hpp"
#include "timer.hpp"
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
    Model(std::unique_ptr<i_Timer> syncTimer,
          std::unique_ptr<i_FileSynchronizer> fileSynchronizer,
          std::unique_ptr<i_Scanner> scanner);

    ErrorCode addDirectory(const std::string &dirName);
    ErrorCode removeDirectory(const std::string &dirName);
    ErrorCode removeFile(const std::string &dirName);
    void createMainDir();

    void setIntervalTime(const std::string &strInterval);
    void setIntervalTime(std::chrono::duration<int64_t, std::milli> interval);
    void startSync();
    void stopSync();
    void forceSync();
    void readConfig();
    void saveConfig();
    fs::path getMainDirectoryPath();

private:
    bool validateForRemoval(std::string name);

    std::unique_ptr<i_Timer> m_syncTimer;
    std::unique_ptr<i_FileSynchronizer> m_fileSynchronizer;
    std::unique_ptr<i_Scanner> m_scanner;
    std::unique_ptr<Serializer> m_serializer;

    std::chrono::duration<int64_t, std::milli> m_interval;
    const fs::path m_mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";
};

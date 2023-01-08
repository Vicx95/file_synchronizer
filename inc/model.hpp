#pragma once

#include "FileSynchronizer.hpp"
#include "timer.hpp"
#include <filesystem>

using Path_t = std::filesystem::path;

enum class ErrorCode
{
    SUCCESS = 0,
    FAIL = 1
};

class Model
{
    const Path_t mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";

public:
    Model(i_Timer *syncTimer, i_FileSynchronizer *fileSynchronizer);

    ErrorCode addDirectory(std::istream &std_input);
    void setIntervalTime(std::istream &std_input);
    ErrorCode removeDirectory();
    ErrorCode removeFile();
    void startSync();
    Path_t getMainDirectoryPath();

private:
    bool validateForRemoval(std::string name);

    i_Timer *m_syncTimer;
    i_FileSynchronizer *m_fileSynchronizer;

    int m_timeInterval = 1000;
    std::chrono::duration<int64_t, std::milli> m_interval;
};

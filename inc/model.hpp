#pragma once

#include "FileSynchronizer.hpp"
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
    Model(i_Timer *syncTimer, i_FileSynchronizer *fileSynchronizer, i_Scanner *scanner);

    ErrorCode addDirectory(std::istream &std_input);
    void setIntervalTime(std::istream &std_input);
    ErrorCode removeDirectory();
    ErrorCode removeFile();
    void startSync();
    fs::path getMainDirectoryPath();

private:
    bool validateForRemoval(std::string name);

    i_Timer *m_syncTimer;
    i_FileSynchronizer *m_fileSynchronizer;
    i_Scanner *m_scanner;

    std::chrono::duration<int64_t, std::milli>
        m_interval;
    const fs::path m_mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";
};

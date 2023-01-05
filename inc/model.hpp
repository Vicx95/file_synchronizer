#pragma once

#include "timer.hpp"
#include <filesystem>

using Path_t = std::filesystem::path;

enum class ErrorCode
{
    SUCCESS = 0,
    FAIL = 1
};

/* void synchronizeFolders()
{
    std::puts("Synchronizuje!");
} */

class Model
{
    const Path_t mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";

public:
    Model(i_Timer *syncTimer);
    ErrorCode addDirectory(std::istream &std_input);
    ErrorCode removeDirectory();
    ErrorCode removeFile();
    void startSync();

    Path_t getMainDirectoryPath();

private:
    bool validationForRemoving(std::string name);
    i_Timer *m_syncTimer;
    int m_timeInterval = 1000;
};

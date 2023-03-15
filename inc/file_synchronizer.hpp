#pragma once

#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;
using PathTimePair_t = std::pair<fs::path, fs::file_time_type>;

class i_FileSynchronizer
{
public:
    virtual ~i_FileSynchronizer() = default;

    virtual void synchronizeAdded(const std::vector<PathTimePair_t> &addedFiles) = 0;
    virtual void synchronizeRemoved(const std::vector<PathTimePair_t> &removedFiles) = 0;
};

class FileSynchronizer : public i_FileSynchronizer
{
public:
    FileSynchronizer() = default;
    ~FileSynchronizer() = default;

    void synchronizeAdded(const std::vector<PathTimePair_t> &addedFiles) override;
    void synchronizeRemoved(const std::vector<PathTimePair_t> &removedFiles) override;
    const fs::path m_mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";
};
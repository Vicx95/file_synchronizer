#pragma once

#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;
using PathTimePair_t = std::pair<fs::path, fs::file_time_type>;

class i_FileSynchronizer
{
public:
    virtual void synchronizeAdded(const std::vector<PathTimePair_t> &addedFiles) = 0;
    virtual void synchronizeRemoved(const std::vector<PathTimePair_t> &removedFiles) = 0;
};

class FileSynchronizer : public i_FileSynchronizer
{
public:
    FileSynchronizer();
    virtual ~FileSynchronizer();

    FileSynchronizer(const FileSynchronizer &) = default;
    FileSynchronizer &operator=(const FileSynchronizer &) = default;
    FileSynchronizer(FileSynchronizer &&) = default;
    FileSynchronizer &operator=(FileSynchronizer &&) = default;

    void synchronizeAdded(const std::vector<PathTimePair_t> &addedFiles) override;
    void synchronizeRemoved(const std::vector<PathTimePair_t> &removedFiles) override;
};
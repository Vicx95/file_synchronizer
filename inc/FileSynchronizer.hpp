#pragma once

#include "Scanner.hpp"
#include "timer.hpp"

class i_FileSynchronizer
{
public:
    virtual ~i_FileSynchronizer(){};
    virtual void synchronizeAdded(const std::vector<PathTimePair_t> &addedFiles) = 0;
    virtual void synchronizeRemoved(const std::vector<PathTimePair_t> &removedFiles) = 0;
};

class FileSynchronizer : public i_FileSynchronizer
{
public:
    void synchronizeAdded(const std::vector<PathTimePair_t> &addedFiles) override;
    void synchronizeRemoved(const std::vector<PathTimePair_t> &removedFiles) override;
};
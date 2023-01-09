#pragma once

#include "Scanner.hpp"
#include "timer.hpp"

using Path_t = std::filesystem::path;

class i_FileSynchronizer
{
public:
    virtual void synchronize(const Path_t &path) = 0;
};

class FileSynchronizer : public i_FileSynchronizer
{
public:
    FileSynchronizer(Scanner *scanner);
    virtual ~FileSynchronizer();

    void synchronize(const Path_t &path) override;

private:
    Scanner *m_scanner;
};
#pragma once

#include "Scanner.hpp"
#include "timer.hpp"

using Path_t = std::filesystem::path;

class i_FileSynchronizer
{
public:
    // virtual void addFolder(const std::string &p_folderName) = 0;
    // virtual void deleteFolder(const std::string &p_folderName) = 0;
    // virtual void deleteFileFrom(const std::string &p_folderName) = 0;

    virtual void synchronize(const Path_t &path) = 0;
};

class FileSynchronizer : public i_FileSynchronizer
{
public:
    FileSynchronizer(Scanner *scanner);
    virtual ~FileSynchronizer();

    // void addFolder(const std::string &p_folderName) override;
    // void deleteFolder(const std::string &p_folderName) override;
    // void deleteFileFrom(const std::string &p_folderName) override;

    void synchronize(const Path_t &path) override;

private:
    // std::vector<std::unique_ptr<Folder>> m_synchronizedFolders;
    Scanner *m_scanner;
};
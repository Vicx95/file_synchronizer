#pragma once

#include "Scanner.hpp"
#include "timer.hpp"

class i_FileSynchronizer
{
public:
    // virtual void addFolder(const std::string &p_folderName) = 0;
    // virtual void deleteFolder(const std::string &p_folderName) = 0;
    // virtual void deleteFileFrom(const std::string &p_folderName) = 0;

    virtual void synchronize() = 0;
};

class FileSynchronizer : public i_FileSynchronizer
{
public:
    FileSynchronizer();
    virtual ~FileSynchronizer();

    // void addFolder(const std::string &p_folderName) override;
    // void deleteFolder(const std::string &p_folderName) override;
    // void deleteFileFrom(const std::string &p_folderName) override;

    void synchronize() override;

private:
    // std::vector<std::unique_ptr<Folder>> m_synchronizedFolders;
    Scanner m_scanner;
    Timer m_timer;
};
#pragma once

#include "Scanner.hpp"
#include "timer.hpp"

class FileSynchronizer
{
public:
    FileSynchronizer();
    // void addFolder(const std::string& p_folderName) {}
    // void deleteFolder(const std::string& p_folderName) {}
    void setTimeInterval();
    // void deleteFileFrom(const std::string& p_folderName) {}

private:
    // std::vector<std::unique_ptr<Folder>> m_synchronizedFolders;
    Scanner m_scanner;
    Timer m_timer;
};
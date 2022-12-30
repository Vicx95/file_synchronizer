#pragma once

#include "Folder.hpp"
#include "Scanner.hpp"

class FileSynchronizer {
public:
    void addFolder(const std::string& p_folderName) {}
    void deleteFolder(const std::string& p_folderName) {}
    void setTimeInterval() {}
    void deleteFileFrom(const std::string& p_folderName) {}

private:
    std::vector<std::unique_ptr<Folder>> m_synchronizedFolders;
    std::vector<std::unique_ptr<Folder>> m_previousSynchronizedFolders;
    Scanner m_scanner;
};
#pragma once

#include <atomic>
#include <filesystem>
#include <iostream>
#include <regex>
#include <set>

namespace fs = std::filesystem;

class View
{

public:
    void printMenu();
    void printOptions();
    void printDirectory(const fs::path &path);
    void printFiles(const std::set<fs::path> &fileDirList);

private:
    bool validateForPrinting(std::string name);
};
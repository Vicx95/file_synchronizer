#pragma once

#include <filesystem>
#include <iostream>
#include <set>
#include <string>

namespace fs = std::filesystem;

class View
{
public:
    View() = default;
    virtual ~View();

    View(const View &) = default;
    View &operator=(const View &) = default;
    View(View &&) = default;
    View &operator=(View &&) = default;

    enum class Message
    {
        Incorrect = 0,
        GiveFolder,
        DirExists,
        Interval,
        Remove,
        RemoveFile,
        PrintFiles,
        FolderEmpty
    };

    void printMenu();
    void printOptions();
    void printDirectory(const fs::path &path);
    void printFiles(const std::set<fs::path> &fileDirList);

    void printMessage(Message m);

private:
    bool validateForPrinting(std::string name);
};
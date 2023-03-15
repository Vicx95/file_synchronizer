#pragma once

#include <filesystem>
#include <ftxui/dom/table.hpp> // for Table, TableSelection
#include <iostream>
#include <set>
#include <string>

namespace fs = std::filesystem;

class View
{
public:
    View() = default;
    ~View() = default;

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

class ViewFTXuserInterface : public View
{
public:
    void run();
    void printDirectory();
    void printFiles();

    std::vector<std::vector<std::string>> printDir();
    std::vector<std::vector<std::string>> printAllFiles();

    std::string get_permission_string(fs::perms permission);
    void generateColorTable(ftxui::Table *table);
    void refreshDir(std::vector<std::string> &dirNames);
};

class ViewConsoleUserInterface : public View
{
public:
    void run();
    void printDirectory();
    void printFiles();
    void printMenu();
    void printOptions();
    static void waitForButton();
    bool validateForPrinting(std::string name);
};
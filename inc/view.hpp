#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <optional>
#include <regex>
#include <filesystem>
#include <iostream>
#include <set>
#include <string>

#include <condition_variable>

//#include <ftxui/dom/table.hpp>            // for Table, TableSelection
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer

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

    virtual void run() = 0;
    void printMenu();
    void printOptions();
    void printDirectory(const fs::path &path);
    void printFiles(const std::set<fs::path> &fileDirList);

    void printMessage(Message m);

protected:
    std::atomic<bool> m_isExitRequested = false;


private:
    bool validateForPrinting(std::string name);

};


class ViewConsoleUserInterface : public View
{
    public:
        void run();
        /*
        void removeFile();
        void printDirectory();
        void printFiles();
        void printMenu();
        void printOptions();
        static void waitForButton();
        bool validateForPrinting(std::string name);
        */

};

class ViewFTXuserInterface : public View
{
    public:
        void run();

        ftxui::Component createButtons(std::deque<bool*> &showButtons, std::condition_variable& cv, std::atomic<bool>& refresh_ui_continue, std::vector<std::string>& dirNamesX, ftxui::ScreenInteractive& screen);
        void hideMenuButtons(std::deque<bool*> &showButtons);
        void refreshDir(std::vector<std::string> &dirNames);


       /* 
        void printDirectory();

        void printFiles();
        void removeFile();


        std::vector<std::vector<std::string>> printDir();
        std::vector<std::vector<std::string>> printAllFiles();

        std::string get_permission_string(fs::perms permission);
        void generateColorTable(ftxui::Table* table);
        void refreshDir(std::vector<std::string> &dirNames);
        void refreshFile(std::vector<std::string> &dirNames, std::vector<std::vector<std::string>> &filesNames);
        void hideMenuButtons(std::deque<bool*> &showButtons);
        */
};
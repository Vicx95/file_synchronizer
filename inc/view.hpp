#pragma once

#include <filesystem>
#include <iostream>
#include <regex>
#include <ftxui/dom/table.hpp>      // for Table, TableSelection

namespace fs = std::filesystem;

class i_ViewListener
{
public:
    virtual void addDirectory(std::istream &std_input) = 0;
    virtual void removeDirectory(std::istream &std_input) = 0;
    virtual void removeFile() = 0;
    virtual std::vector<std::vector<std::string>> printDirectory() = 0;
    virtual std::vector<std::vector<std::string>> printFiles() = 0;
    virtual void setIntervalTime(std::istream &std_input) = 0;
    virtual void startSync() = 0;
    virtual void stopSync() = 0;    
    virtual void forceSync() = 0;
    virtual void readConfig() = 0;
    virtual void saveConfig() = 0;
    virtual bool exit() = 0;
};

class View
{

public:
    enum class Action
    {
        AddDir = 1,
        RemoveDir = 2,
        RemoveFile = 3,
        PrintDir = 4,
        PrintFiles = 5,
        SetIntervalTime = 6,
        StartSync = 7,
        StopSync = 8,
        ForceSync = 9,
        readConfig = 10,
        saveConfig = 11,
        Exit = 0
    };

    void setListener(i_ViewListener *listener);
    virtual void run() = 0;

    virtual std::vector<std::vector<std::string>> printDirectory() = 0;
    virtual std::vector<std::vector<std::string>> printFiles() = 0;

    void setMainDirectoryPath(const fs::path &path);


protected:
    fs::path mainDirectoryPath;

    bool m_isExitRequested = false;
    i_ViewListener *listener;
};


class ViewFTXuserInterface : public View
{
    public:
        void run();
        std::vector<std::vector<std::string>> printDirectory();
        std::vector<std::vector<std::string>> printFiles();

        std::string get_permission_string(fs::perms permission);
        void generateColorTable(ftxui::Table* table);
        void refreshDir(std::vector<std::string> &dirNames);

};

class ViewConsoleUserInterface : public View
{
    public:
        void run();
        std::vector<std::vector<std::string>> printDirectory();
        std::vector<std::vector<std::string>> printFiles();


        void printMenu();
        void printOptions();
        static void waitForButton();
        bool validateForPrinting(std::string name);

};
#pragma once

#include <filesystem>
#include <iostream>
#include <regex>

namespace fs = std::filesystem;

class i_ViewListener
{
public:
    virtual void addDirectory(std::istream &std_input) = 0;
    virtual void removeDirectory() = 0;
    virtual void removeFile() = 0;
    virtual void printDirectory() = 0;
    virtual void printFiles() = 0;
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
    void run();
    void printMenu();
    void printOptions();
    void printDirectory();
    void printFiles();
    static void waitForButton();
    void setMainDirectoryPath(const fs::path &path);

private:
    bool validateForPrinting(std::string name);
    fs::path mainDirectoryPath;

    bool m_isExitRequested = false;
    i_ViewListener *listener;
};
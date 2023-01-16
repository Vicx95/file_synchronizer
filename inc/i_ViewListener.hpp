#pragma once
#include <filesystem>
#include <iostream>

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

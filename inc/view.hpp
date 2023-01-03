#pragma once

#include <iostream>

class i_ViewListener
{
public:
	virtual void addDirectory(std::cin) = 0;
    virtual void removeDirectory() = 0;
    virtual void removeFile() = 0;
    virtual void printDirectory() = 0;
    virtual void printFiles() = 0;
    virtual void setIntervalTime() = 0;
    virtual void startSync() = 0;
    virtual void forceSync() = 0;
    virtual void exit() = 0;
};

class View
{
	i_ViewListener* listener;
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
        ForceSync = 8,
        Exit = 0 
    };

	void setListener(i_ViewListener* listener);
	void run();
    void printMenu();
    void printOptions();
    void printDirectory();
    void printFiles();
    static void waitForButton();

private:
    bool validationForPrinting(std::string name);

};
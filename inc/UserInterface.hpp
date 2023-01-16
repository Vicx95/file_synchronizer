
#pragma once
#include "i_View.hpp"
#include "i_ViewListener.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


class UserInterface  {
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

    UserInterface();

    virtual void run() = 0;
    virtual void printMenu() = 0;
    virtual void printOptions() = 0;
    virtual void printDirectory() = 0;
    virtual void printFiles() = 0;
  //  void setListener(i_ViewListener *listener);
  //  i_ViewListener* getListener();
   // void setMainDirectoryPath(const fs::path &path);

    static void waitForButton();
    virtual ~UserInterface() {}
    bool m_isExitRequested = false;



   // fs::path mainDirectoryPath;

    i_View *m_view;
    i_ViewListener *m_listener;

};


class ConsoleUserInterface : public UserInterface{
public:
   // ConsoleUserInterface(View *view, i_ViewListener *listener);
    void run() override;
    void printMenu() override;
    void printOptions() override;
    void printDirectory() override;
    void printFiles() override;

private:

    //View *m_view;
    //i_ViewListener *m_listener;
};


class FTXuserInterface : public UserInterface{
public:
    void run() override;
    void printMenu() override;
    void printOptions() override;
    void printDirectory() override;
    void printFiles() override;
};

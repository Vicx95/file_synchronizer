#pragma once
#include "..//inc/UserInterface.hpp"
#include "i_View.hpp"
#include <filesystem>
#include <iostream>
#include <regex>
#include <memory>

namespace fs = std::filesystem;



class View : public i_View
{

public:

    View();
    void setListener(i_ViewListener *listener);
    i_ViewListener* getListener() override;
    void run() override;
   // void printMenu();
   // void printOptions();
    void printDirectory() override;
   // void printFiles();
   // static void waitForButton();
    void setMainDirectoryPath(const fs::path &path) override;
    fs::path getMainDirectoryPath() override;
     bool m_isExitRequested = false;
    i_ViewListener *listener;

private:
    bool validateForPrinting(std::string name);
    fs::path mainDirectoryPath;

  // UserInterface ui;
    std::unique_ptr<UserInterface> ui = std::make_unique<ConsoleUserInterface>();
 //   std::unique_ptr<UserInterface> ui = std::make_unique<FTXuserInterface>();

};
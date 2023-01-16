#pragma once
#include <filesystem>
#include "i_ViewListener.hpp"
#include <iostream>
#include <regex>
#include <memory>

namespace fs = std::filesystem;


class i_View{
public:
    virtual i_ViewListener* getListener() = 0;
    virtual void run() = 0;
   // void printMenu();
   // void printOptions();
    virtual void printDirectory() = 0;
   // void printFiles();
   // static void waitForButton();
    virtual void setMainDirectoryPath(const fs::path &path) = 0;
    virtual fs::path getMainDirectoryPath() = 0;

};

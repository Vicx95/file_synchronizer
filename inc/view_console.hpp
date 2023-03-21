#pragma once

#include "view.hpp"

class ViewConsoleUserInterface : public View
{
public:
    void run(const fs::path &path);
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

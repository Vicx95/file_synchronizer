#pragma once

#include "view.hpp"

class ViewConsoleUserInterface : public View
{
public:
    void run(const fs::path &path) override;
    std::string getTypeUI() override;
    void setModel(i_Model *ptr) override;

    void removeFile();
    void printDirectory();
    void printFiles();
    void printMenu();
    void printOptions();
    static void waitForButton();
    bool validateForPrinting(std::string name);
    
};

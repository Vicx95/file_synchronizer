#include "inc/view_console.hpp"

void ViewConsoleUserInterface::setModel(i_Model *ptr)
{
 (void)(*ptr);
}

void ViewConsoleUserInterface::printMenu()
{
    std::cout << "\n\n ### FILE SYNCHRONIZER ### \n\n";
}

void ViewConsoleUserInterface::printOptions()
{
    std::cout << "0. Exit \n-------------------------\n"
              << "1. Add new directory \n"
              << "2. Remove directory \n"
              << "3. Remove file \n"
              << "4. Print all directories \n"
              << "5. Print all files \n"
              << "6. Set interval time  \n"
              << "7. Start sync-up  \n"
              << "8. Stop sync-up  \n"
              << "9. Force sync-up  \n"
              << "10. Read config  \n"
              << "11. Save config  \n";
}

std::string ViewConsoleUserInterface::getTypeUI()
{
    return "Console";
}

void ViewConsoleUserInterface::run(const fs::path &path)
{
    fs::path spath = path;
    printMenu();
    printOptions();
}
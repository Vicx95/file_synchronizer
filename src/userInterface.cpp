#include <iostream>
#include <fstream>
#include <ostream>
#include <filesystem>
#include <set>

#include "..//inc/userInterface.hpp"

namespace fs = std::filesystem;

UserInterface::UserInterface() 
{
    if(fs::exists(mainDirectoryPath))
    {
        fs::current_path(mainDirectoryPath);
    }
    else
    {
        fs::current_path(fs::current_path().parent_path());
        fs::create_directory("mainDirectory");
        fs::current_path(mainDirectoryPath);
    }
}

void UserInterface::startSynchronizer() {
    while (true) {
        printMenu();
        printOptions();
        uint32_t action;
        std::cin.clear();
        std::cin >> action;
        if (!action)
            break;
        makeAction(static_cast<Action>(action));
        std::system("clear");

    }
}

void UserInterface::printOptions() {
    std::cout << "0. Exit \n-------------------------\n"
              << "1. Add new directory \n"
              << "2. Remove directory \n"
              << "3. Remove file \n"
              << "4. Print all directories \n"
              << "5. Print all files \n"
              << "6. Set interval time  \n"
              << "7. Force sync-up  \n";
}

void UserInterface::makeAction(Action choice)
{
    switch(choice) 
    {
        case Action::AddDir:
            std::cout << "added directory \n";
            addDirectory();
            break;
        case Action::RemoveDir:
            std::cout << "Removed directory \n";
            removeDirectory();
            break;
        case Action::RemoveFile:
            std::cout << "Removed file \n";
            removeFile();
            break;
        case Action::PrintDir:
            printDirectory();
            break;
        case Action::PrintFiles:
            printFiles();
            break;
        case Action::SetIntervalTime:
            break;
        case Action::ForceSync:
            break;
        case Action::Exit:
            break;
        default:
            break;
    }

}

void UserInterface::printMenu() 
{
    std::cout << "### FILE SYNCHRONIZER ### \n\n";
}

void UserInterface::printDirectory()
{
    fs::current_path(mainDirectoryPath);
    std::cout << "Print current path: " << mainDirectoryPath << "\n\n";

    for (auto const& dirEntry : fs::directory_iterator(mainDirectoryPath))
    {
        std::cout << dirEntry.path() << std::endl;
    }
    if(fs::is_empty(mainDirectoryPath))
        std::cout << "Folder is empty...\n";
    waitForButton();
} 

void UserInterface::printFiles()
{
    fs::current_path(mainDirectoryPath);
    std::cout << "Give folder name or choose 'all' to print files: \n";
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;

    if(dirName == "all")
        dirName = mainDirectoryPath;
    
    for (auto const& dirEntry : fs::recursive_directory_iterator(dirName))
    {
        std::cout << dirEntry.path().filename() << "\n-------------------\n";
        //std::cout << dirEntry.path() << "\n";
    }

    if(fs::is_empty(mainDirectoryPath / dirName))
        std::cout << "Folder is empty...";

    std::cout << "\n";
    waitForButton();
}

void UserInterface::addDirectory()
{
    //fs::current_path(mainDirectoryPath);

    std::cout << "Give folder name to add: \n";
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;
    fs::create_directory(dirName);

}

void UserInterface::removeDirectory()
{
    //fs::current_path(mainDirectoryPath);
    std::cout << "Give folder name to remove: \n";
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;
    validationForRemoving(dirName);
    try
    {
        fs::remove_all(dirName);
    }
    catch(std::filesystem::filesystem_error const& ex)
    {
        std::cerr << ex.what() << '\n';
    }
    
    
}

void UserInterface::removeFile()
{
    std::cout << "Give folder name with files to delete: \n";
    fs::current_path(mainDirectoryPath);
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;
    if(validationForRemoving(dirName))
        fs::current_path(mainDirectoryPath / dirName);    
    std::cout << "Give file name to remove: \n";
    std::string fileName;
    std::cin.clear();
    std::cin >> fileName;
    validationForRemoving(fileName);
    fs::remove(fileName);
}

void UserInterface::waitForButton()
{
    std::cout << "\nPress any button to continue... \n";
    std::string button;
    std::cin.clear();
    std::cin >> button;
}

bool UserInterface::validationForRemoving(std::string name)
{
    if(!fs::exists(mainDirectoryPath / name))
    {
        std::cout << "Given folder/file name not exist...\n";
        waitForButton();
        return false;
    }
    return true;
}
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <set>

#include "..//inc/userInterface.hpp"

namespace fs = std::filesystem;

UserInterface::UserInterface() {
    if (fs::exists(mainDirectoryPath)) {
        fs::current_path(mainDirectoryPath);
    } else {
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

void UserInterface::makeAction(Action choice) {
    switch (choice) {
    case UserInterface::Action::AddDir:
        std::cout << "added directory \n";
        addDirectory(std::cin);
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
        std::cout << "Incorrect action selected! Please try again...\n";
        waitForButton();
        break;
    }
}

void UserInterface::printMenu() {
    std::cout << "### FILE SYNCHRONIZER ### \n\n";
}

void UserInterface::printDirectory() {
    fs::current_path(mainDirectoryPath);
    std::cout << "Print current path: " << mainDirectoryPath << "\n\n";

    for (auto const& dirEntry : fs::directory_iterator(mainDirectoryPath)) {
        std::cout << dirEntry.path() << std::endl;
    }
    if (fs::is_empty(mainDirectoryPath))
        std::cout << "Folder is empty...\n";
    waitForButton();
}

void UserInterface::printFiles() {
    std::set<fs::path> sorted_by_name;
    std::cout << "Give folder name or choose 'all' to print files: \n";
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;

    if (validationForPrinting(dirName)) {
        if (dirName == "all") {
            dirName = mainDirectoryPath;
        }
        for (auto const& dirEntry : fs::recursive_directory_iterator(dirName)) {
            sorted_by_name.insert(dirEntry.path());
        }

        for (auto const& fileName : sorted_by_name) {
            if (fs::is_directory(fileName)) {
                std::cout << "-------------------\n";
                std::cout << "Directory: " << fileName.filename() << ":\n";
            }
            if (fs::is_regular_file(fileName)) {
                std::cout << fileName.filename() << "\n";
            }
        }

        if (fs::is_empty(mainDirectoryPath / dirName))
            std::cout << "Folder is empty...";
    
    std::cout << "\n";
    waitForButton();
    }

}

ErrorCode UserInterface::addDirectory(std::istream &std_input) {
    fs::current_path(mainDirectoryPath);

    std::cout << "Give folder name to add: \n";
    std::string dirName;
    //std::cin.clear();
    //std::cin >> dirName;
    std_input >> dirName;
    if (!fs::exists(fs::current_path() / dirName))
    {
        fs::create_directory(dirName);
        return ErrorCode::SUCCESS;
    }
    else
    {
        std::cout << "Dir already exist...\n";
        waitForButton();
        return ErrorCode::FAIL;
    }
}

ErrorCode UserInterface::removeDirectory() {
    fs::current_path(mainDirectoryPath);
    std::cout << "Give folder name to remove: \n";
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;
    if (validationForRemoving(dirName)) {
        fs::remove_all(dirName);
        return ErrorCode::SUCCESS;
    }
    return ErrorCode::FAIL;
}

ErrorCode UserInterface::removeFile() {
    std::cout << "Give folder name with files to delete: \n";
    fs::current_path(mainDirectoryPath);
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;
    if (validationForRemoving(dirName)) {
        fs::current_path(mainDirectoryPath / dirName);
        std::cout << "Give file name to remove: \n";
        std::string fileName;
        std::cin.clear();
        std::cin >> fileName;
        if (validationForRemoving(fileName)) {
            fs::remove(fileName);
            return ErrorCode::SUCCESS;
        }
    }
    return ErrorCode::FAIL;
}

void UserInterface::waitForButton() {
    system("read -n 1 -s -p \"Press any key to continue...\"");
}

bool UserInterface::validationForRemoving(std::string name) {
    if (!fs::exists(fs::current_path() / name)) {
        std::cout << "Not exist file or directory\n";
        waitForButton();
        return false;
    }
    return true;
}

bool UserInterface::validationForPrinting(std::string name) {
    if (name == "all") {
        return true;
    }
    if (!fs::exists(fs::current_path() / name)) {
        std::cout << "Not exist file or directory\n";
        waitForButton();
        return false;
    }
    return true;
}
#include "..//inc/view.hpp"
#include "..//inc/model.hpp"

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <set>
#include <thread>

namespace fs = std::filesystem;


void View::setListener(i_ViewListener *listener)
{
    this->listener = listener;
}

void View::printMenu()
{
    std::cout << "\n\n ### FILE SYNCHRONIZER ### \n\n";
}

void View::printOptions()
{
    std::cout << "0. Exit \n-------------------------\n"
              << "1. Add new directory \n"
              << "2. Remove directory \n"
              << "3. Remove file \n"
              << "4. Print all directories \n"
              << "5. Print all files \n"
              << "6. Set interval time  \n"
              << "7. Start sync-up  \n"
              << "8. Force sync-up  \n"
              << "9. Read config  \n"
              << "10. Save config  \n";
}

void View::waitForButton()
{
    std::system("/bin/bash -c \"read -n 1 -s -p \"PressAnyKeyToContinue...\"\"");
}

void View::printDirectory()
{

    fs::current_path(mainDirectoryPath);
    std::cout << "Print current path: " << mainDirectoryPath << "\n\n";

    for (auto const &dirEntry : fs::directory_iterator(mainDirectoryPath))
    {
        std::cout << dirEntry.path() << std::endl;
    }
    if (fs::is_empty(mainDirectoryPath))
        std::cout << "Folder is empty...\n";
    waitForButton();
}

bool View::validateForPrinting(std::string name)
{
    if (name == "all")
    {
        return true;
    }
    if (!fs::exists(fs::current_path() / name))
    {
        std::cout << "Not exist file or directory\n";
        waitForButton();
        return false;
    }
    return true;
}

void View::printFiles()
{
    std::set<fs::path> sorted_by_name;
    std::cout << "Give folder name or choose 'all' to print files: \n";
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;

    if (validateForPrinting(dirName))
    {
        if (dirName == "all")
        {
            dirName = mainDirectoryPath;
        }
        for (auto const &dirEntry : fs::recursive_directory_iterator(dirName))
        {
            sorted_by_name.insert(dirEntry.path());
        }

        for (auto const &fileName : sorted_by_name)
        {
            if (fs::is_directory(fileName))
            {
                std::cout << "-------------------\n";
                std::cout << "Directory: " << fileName.filename() << ":\n";
            }
            if (fs::is_regular_file(fileName))
            {
                std::cout << fileName.filename() << "\n";
            }
        }

        if (fs::is_empty(mainDirectoryPath / dirName))
            std::cout << "Folder is empty...";

        std::cout << "\n";
        waitForButton();
    }
}

void View::run()
{
    std::string inputKey;
    std::regex keyRegex("([0-9]{1})");

    while (!m_isExitRequested)
    {
        printMenu();
        printOptions();

        std::cin.clear();
        std::cin >> inputKey;

        if (!std::regex_search(inputKey, keyRegex))
        {
            std::cout << "Incorrect action selected! Please try again...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000000));
            //usleep(1000000); // would be nice to replace it with std::this_thread::sleep_for()
            continue;
        }

        switch (static_cast<Action>(std::stoul(inputKey)))
        {
        case Action::AddDir:
            std::cout << "added directory \n";
            listener->addDirectory(std::cin);
            break;
        case Action::RemoveDir:
            std::cout << "Removed directory \n";
            listener->removeDirectory();
            break;
        case Action::RemoveFile:
            std::cout << "Removed file \n";
            listener->removeFile();
            break;
        case Action::PrintDir:
            listener->printDirectory();
            break;
        case Action::PrintFiles:
            listener->printFiles();
            break;
        case Action::SetIntervalTime:
            listener->setIntervalTime(std::cin);
            break;
        case Action::StartSync:
            listener->startSync();
            break;
        case Action::ForceSync:
            listener->forceSync();
            break;
        case Action::readConfig:
            listener->readConfig();
            break;
        case Action::saveConfig:
            listener->saveConfig();
            break;
        case Action::Exit:
            m_isExitRequested = listener->exit();
            break;
        default:
            std::cout << "Incorrect action selected! Please try again...\n";
            waitForButton();
            break;
        }
    }
}

void View::setMainDirectoryPath(const Path_t &path)
{
    mainDirectoryPath = path;
}
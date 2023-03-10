#include "..//inc/view.hpp"
#include "..//inc/model.hpp"

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <set>
#include <thread>

namespace fs = std::filesystem;

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
              << "8. Stop sync-up  \n"
              << "9. Force sync-up  \n"
              << "10. Read config  \n"
              << "11. Save config  \n";
}

void View::printDirectory(const fs::path &path)
{
    fs::current_path(path);
    std::cout << "Print current path: " << path << "\n\n";

    for (auto const &dirEntry : fs::directory_iterator(path))
    {
        std::cout << dirEntry.path() << std::endl;
    }
    if (fs::is_empty(path))
        std::cout << "Folder is empty...\n";
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
        return false;
    }
    return true;
}

void View::printFiles(const std::set<fs::path> &fileDirList)
{
    (void)fileDirList;
    /*     std::set<fs::path> sorted_by_name;
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
        } */
}
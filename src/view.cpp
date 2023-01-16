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

i_ViewListener* View::getListener(){
    return listener;
}


void View::printDirectory()
{
    ui->printDirectory();
    /*
    fs::current_path(mainDirectoryPath);
    std::cout << "Print current path: " << mainDirectoryPath << "\n\n";

    for (auto const &dirEntry : fs::directory_iterator(mainDirectoryPath))
    {
        std::cout << dirEntry.path() << std::endl;
    }
    if (fs::is_empty(mainDirectoryPath))
        std::cout << "Folder is empty...\n";
    //waitForButton();
    */
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
        //waitForButton();
        return false;
    }
    return true;
}
/*
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
        //waitForButton();
    }
}
*/
void View::run()
{
    ui->run();
}

void View::setMainDirectoryPath(const fs::path &path)
{
    mainDirectoryPath = path;
}

fs::path View::getMainDirectoryPath()
{
    return mainDirectoryPath;
}

View::View()
{
    //ui = std::make_unique<ConsoleUserInterface>();
}
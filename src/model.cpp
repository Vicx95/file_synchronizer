// .cpp file
#include "..//inc/model.hpp"
#include "..//inc/view.hpp"

#include <iostream>

namespace fs = std::filesystem;

Model::Model(i_Timer& syncTimer) : m_syncTimer(syncTimer) 
{

};

ErrorCode Model::addDirectory(std::istream &std_input) {
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
        View::waitForButton();
        return ErrorCode::FAIL;
    }
}

bool Model::validationForRemoving(std::string name) {
    if (!fs::exists(fs::current_path() / name)) {
        std::cout << "Not exist file or directory\n";
        View::waitForButton();
        return false;
    }
    return true;
}

ErrorCode Model::removeDirectory() {
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

ErrorCode Model::removeFile() {
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

void Model::startSync()
{
    m_syncTimer.start(std::chrono::milliseconds(1000), [] {
        std::puts("Synchronizuje!");
    });
      
}


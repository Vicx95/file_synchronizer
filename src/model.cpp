// .cpp file
#include "..//inc/model.hpp"
#include "..//inc/view.hpp"

#include <chrono>
#include <iostream>

namespace fs = std::filesystem;

Model::Model(i_Timer *syncTimer, i_FileSynchronizer *fileSynchronizer, i_Scanner *scanner)
    : m_syncTimer(syncTimer), m_fileSynchronizer(fileSynchronizer), m_scanner(scanner), m_interval(1000){

};

ErrorCode Model::addDirectory(std::istream &std_input)
{

    try
    {
        fs::current_path(m_mainDirectoryPath);
    }
    catch (std::filesystem::filesystem_error const &ex)
    {
        std::cout << ex.code().message() << '\n';
        fs::create_directory(m_mainDirectoryPath);
        fs::current_path(m_mainDirectoryPath);
    }

    std::cout << "Give folder name to add: \n";
    std::string dirName;
    std_input.clear();
    std_input >> dirName;
    if (!fs::exists(fs::current_path() / dirName))
    {
        fs::create_directory(dirName);
        return ErrorCode::SUCCESS;
    }
    else
    {
        std::cout << "Dir already exist...\n";
        ViewConsoleUserInterface::waitForButton();
        return ErrorCode::FAIL;
    }
}

void Model::setIntervalTime(std::istream &std_input)
{
    std::cout << "Interval time value [milliseconds]: \n";
    int64_t input;

    std_input.clear();
    std_input >> input;

    std::chrono::duration<int64_t, std::milli> _interval(input);
    m_interval = _interval;
}

bool Model::validateForRemoval(std::string name)
{
    if (!fs::exists(fs::current_path() / name))
    {
        std::cout << "Not exist file or directory\n";
        ViewConsoleUserInterface::waitForButton();
        return false;
    }
    return true;
}

ErrorCode Model::removeDirectory(std::istream &std_input)
{
    fs::current_path(m_mainDirectoryPath);
    std::cout << "Give folder name to remove: \n";
    std::string dirName;
    std::cin.clear();
    std_input >> dirName;
    if (validateForRemoval(dirName))
    {
        fs::remove_all(dirName);
        return ErrorCode::SUCCESS;
    }
    return ErrorCode::FAIL;
}

ErrorCode Model::removeFile()
{
    std::cout << "Give folder name with files to delete: \n";
    fs::current_path(m_mainDirectoryPath);
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;
    if (validateForRemoval(dirName))
    {
        fs::current_path(m_mainDirectoryPath / dirName);
        std::cout << "Give file name to remove: \n";
        std::string fileName;
        std::cin.clear();
        std::cin >> fileName;
        if (validateForRemoval(fileName))
        {
            fs::remove(fileName);
            return ErrorCode::SUCCESS;
        }
    }
    return ErrorCode::FAIL;
}

void Model::startSync()
{
    if(!m_syncStarted){
        m_syncStarted = true;
        m_syncTimer->start(m_interval, [this]() {
            forceSync();
        });
    }
}

void Model::stopSync()
{
    m_syncStarted = false;
    m_syncTimer->stop();
}

void Model::forceSync()
{
        m_scanner->scan(m_mainDirectoryPath);
        auto outputComparing = m_scanner->comparePreviousAndRecentScanning();
        m_fileSynchronizer->synchronizeAdded(outputComparing.first);
        m_scanner->scanForChangedDirs(m_mainDirectoryPath);
        m_fileSynchronizer->synchronizeRemoved(outputComparing.second);
        m_scanner->scanForChangedDirs(m_mainDirectoryPath);

}

fs::path Model::getMainDirectoryPath()
{
    return m_mainDirectoryPath;
}


void Model::readConfig(std::istream &std_input){
    std::cout << "Read back-up configuration or add new configuration - Select 1 or 2: \n"
              << "1. Add new config \n"
              << "2. Read back-up \n";
    int selected;
    std::cin.clear();
    std_input >> selected;
    std::cout << "Provided: " << selected << "\n";

    if(selected == 1){
        addConfig(std::cin);
    }
    else if(selected == 2){
        m_serializer = std::make_unique<SerializerToJSON>(); 
        fs::remove_all(m_mainDirectoryPath);
        fs::create_directory(m_mainDirectoryPath);
        auto [dirs, files] = m_serializer->deserialize();
        std::filesystem::copy(m_mainDirectoryPath / "../configDirectory", m_mainDirectoryPath, std::filesystem::copy_options::recursive);
    }
    else{
        std::cout << "Provided incorrect value!\n";
        ViewConsoleUserInterface::waitForButton();
    }
}

void Model::saveConfig(){
    fs::remove_all(std::filesystem::current_path() / "../configDirectory");
    std::filesystem::copy(m_mainDirectoryPath, m_mainDirectoryPath / "../configDirectory", std::filesystem::copy_options::recursive);
    std::vector<std::unique_ptr<Serializer>> configurations;
    configurations.emplace_back(std::make_unique<SerializerToJSON>());
    configurations.emplace_back(std::make_unique<SerializerToTxt>());

    for(auto const& config : configurations){
        config->serialize();
    }    
}

void Model::addConfig(std::istream &std_input){

    std::cout << "Provide path with configuration: \n";
    fs::path dirPath;
    std::cin.clear();
    std_input >> dirPath;
    std::cout << "Provided path: " << dirPath << "\nFiles in directory: \n";

    if (!std::filesystem::exists(dirPath)){
        std::cout << "Incorrect path! Directory not exist! \n";
        ViewConsoleUserInterface::waitForButton();
    } 
    else{
        for (auto const &fileEntry : fs::recursive_directory_iterator(dirPath))
        {
            if (fileEntry.is_regular_file()){
                std::cout << fileEntry.path().filename() << "\n";
            }
        }
    
        std::cout << "Provide in JSON format (example: {\"files\": [\"file11.txt\", \"file12.txt\"]}) which files you want to transfer: \n";
        
        json fileJSON;
        std::vector<std::string> Files;

        try{
            std::cin.clear();
            std_input >> fileJSON;
        
            for (json::iterator it = fileJSON.begin(); it != fileJSON.end(); ++it) {
                for(auto file : it.value()){
                    Files.push_back(file);
                }
            }

            std::cout << "Provide copy direction from available dirs: \n";
            for (auto const &dirEntry : fs::directory_iterator(m_mainDirectoryPath))
            {
                std::cout << dirEntry.path().filename() << "\n";
            }

            std::string dir;
            std::cin.clear();
            std_input >> dir;
           
            if (!std::filesystem::exists(m_mainDirectoryPath / dir)){
                std::cout << "Incorrect path! Directory not exist! \n";
                ViewConsoleUserInterface::waitForButton();
            }
            else{
                for(auto file : Files)
                {
                    std::cout << file << "\n";
                    try{
                        std::filesystem::copy(dirPath / file, m_mainDirectoryPath / dir / file, std::filesystem::copy_options::recursive);
                    }
                    catch (std::exception &e){
                        std::cout << e.what();
                    }
                }
            }
        }
        catch (json::parse_error& e)
        {
            std::cerr << e.what() << std::endl;
            ViewConsoleUserInterface::waitForButton();

        }
    }
/*
/Users/pawellos/projects/file_synchronizer/tmpConfig
{"files": ["file11.txt", "file14.txt"]}

*/        
}
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
        View::waitForButton();
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
        View::waitForButton();
        return false;
    }
    return true;
}

ErrorCode Model::removeDirectory()
{
    fs::current_path(m_mainDirectoryPath);
    std::cout << "Give folder name to remove: \n";
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;
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
    m_syncTimer->start(m_interval, [this]() {
        m_scanner->scan(m_mainDirectoryPath);
        auto outputComparing = m_scanner->comparePreviousAndRecentScanning();
        m_fileSynchronizer->synchronizeAdded(outputComparing.first);
        m_scanner->scanForChangedDirs(m_mainDirectoryPath);
        m_fileSynchronizer->synchronizeRemoved(outputComparing.second);
        m_scanner->scanForChangedDirs(m_mainDirectoryPath);
    });
}

void Model::stopSync()
{
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


void Model::readConfig(){
    m_serializer = std::make_unique<SerializerToJSON>(); 
    fs::remove_all(m_mainDirectoryPath);
    fs::create_directory(m_mainDirectoryPath);
    auto [dirs, files] = m_serializer->deserialize();
    std::filesystem::copy(m_mainDirectoryPath / "../configDirectory", m_mainDirectoryPath, std::filesystem::copy_options::recursive);
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

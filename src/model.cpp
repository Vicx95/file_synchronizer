// .cpp file
#include "..//inc/model.hpp"
#include "..//inc/thread_pool_provider.hpp"
#include "..//inc/view.hpp"

#include <chrono>
#include <iostream>

namespace fs = std::filesystem;

Model::Model(std::unique_ptr<i_Timer> syncTimer,
             std::unique_ptr<i_FileSynchronizer> fileSynchronizer,
             std::unique_ptr<i_Scanner> scanner)
    : m_syncTimer(std::move(syncTimer)), m_fileSynchronizer(std::move(fileSynchronizer)), m_scanner(std::move(scanner)), m_interval(1000){

                                                                                                                         };

ErrorCode Model::addDirectory(const std::string &dirName)
{
    if (!fs::exists(fs::current_path() / dirName))
    {
        fs::create_directory(dirName);
        return ErrorCode::SUCCESS;
    }

    return ErrorCode::FAIL;
}

void Model::createMainDir()
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
}

void Model::setIntervalTime(const std::string &strInterval)
{
    m_interval = static_cast<std::chrono::duration<int64_t, std::milli>>(std::stoll(strInterval));
}

void Model::setIntervalTime(std::chrono::duration<int64_t, std::milli> interval)
{
    m_interval = interval;
}

bool Model::validateForRemoval(std::string name)
{
    fs::current_path(m_mainDirectoryPath);

    return !fs::exists(fs::current_path() / name);
}

ErrorCode Model::removeDirectory(const std::string &dirName)
{
    if (validateForRemoval(dirName))
    {
        fs::remove_all(dirName);
        return ErrorCode::SUCCESS;
    }
    return ErrorCode::FAIL;
}

ErrorCode Model::removeFile(const std::string &dirName)
{
    // TODO finish refactoring
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
        this->forceSync();
    });
}

void Model::stopSync()
{
    m_syncTimer->stop();
}

void Model::forceSync()
{
    auto future = ThreadPoolProvider::instance().getThreadPool()->submit([&]() { //
        m_scanner->scan(m_mainDirectoryPath);
        return m_scanner->comparePreviousAndRecentScanning();
    });
    auto outputComparing = future.get();

    // TODO integrate thread pool
    m_fileSynchronizer->synchronizeAdded(outputComparing.first);
    m_fileSynchronizer->synchronizeRemoved(outputComparing.second);
    m_scanner->scanForChangedDirs(m_mainDirectoryPath);
}

fs::path Model::getMainDirectoryPath()
{
    return m_mainDirectoryPath;
}

void Model::readConfig()
{
    m_serializer = std::make_unique<SerializerToJSON>();
    fs::remove_all(m_mainDirectoryPath);
    fs::create_directory(m_mainDirectoryPath);
    auto [dirs, files] = m_serializer->deserialize();
    std::filesystem::copy(m_mainDirectoryPath / "../configDirectory", m_mainDirectoryPath, std::filesystem::copy_options::recursive);
}

void Model::saveConfig()
{
    fs::remove_all(std::filesystem::current_path() / "../configDirectory");
    std::filesystem::copy(m_mainDirectoryPath, m_mainDirectoryPath / "../configDirectory", std::filesystem::copy_options::recursive);
    std::vector<std::unique_ptr<Serializer>> configurations;
    configurations.emplace_back(std::make_unique<SerializerToJSON>());
    configurations.emplace_back(std::make_unique<SerializerToTxt>());

    for (auto const &config : configurations)
    {
        config->serialize();
    }
}

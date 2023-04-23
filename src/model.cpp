#include "..//inc/model.hpp"
#include "..//inc/file_synchronizer.hpp"
#include "..//inc/scanner.hpp"
#include "..//inc/serializer.hpp"
#include "..//inc/thread_pool_provider.hpp"
#include "..//inc/timer.hpp"

namespace fs = std::filesystem;

Model::Model()
    : Model(std::make_unique<Timer>(), std::make_unique<FileSynchronizer>(), std::make_unique<Scanner>(), std::make_unique<SerializerToJSON>(), std::make_unique<Stream>())
{
}

Model::Model(std::unique_ptr<i_Timer> syncTimer,
             std::unique_ptr<i_FileSynchronizer> fileSynchronizer,
             std::unique_ptr<i_Scanner> scanner,
             std::unique_ptr<i_Serializer> serializer,
             std::unique_ptr<Stream> stream) noexcept
    : m_syncTimer(std::move(syncTimer)),               //
      m_fileSynchronizer(std::move(fileSynchronizer)), //
      m_scanner(std::move(scanner)),                   //
      m_serializer(std::move(serializer)),             //
      m_stream(std::move(stream)),
      m_interval(1000)                                 //
      {

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
    return fs::exists(fs::current_path() / name);
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
    if (validateForRemoval(dirName))
    {
        fs::current_path(m_mainDirectoryPath / dirName);
        std::cout << "Give file name to remove: \n";
        std::string fileName;
        std::cin.clear();
        std::cin >> fileName;
        if (validateForRemoval(dirName + "/" + fileName))
        {
            fs::remove(m_mainDirectoryPath / dirName / fileName);

            return ErrorCode::SUCCESS;
        }
    }
    return ErrorCode::FAIL;
}

ErrorCode Model::getAllFilesInDir(const std::string &dirName, std::set<fs::path> &fileList)
{
    if (!fs::is_empty(dirName))
    {
        for (auto const &dirEntry : fs::recursive_directory_iterator(dirName))
        {
            fileList.insert(dirEntry.path());
        }

        return ErrorCode::SUCCESS;
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
    fs::remove_all(m_mainDirectoryPath);
    fs::create_directory(m_mainDirectoryPath);
    fs::path configPath = m_mainDirectoryPath / "config.json";
    //auto [dirs, files] = m_serializer->deserialize(configPath);
    std::filesystem::copy(m_mainDirectoryPath / "../configDirectory", m_mainDirectoryPath, std::filesystem::copy_options::recursive);
}

void Model::saveConfig()
{
    fs::remove_all(std::filesystem::current_path() / "../configDirectory");
    std::filesystem::copy(m_mainDirectoryPath, m_mainDirectoryPath / "../configDirectory", std::filesystem::copy_options::recursive);

    m_serializer->serialize();
}

void Model::setupStreaming()
{
    m_stream->loadStreaming();
}

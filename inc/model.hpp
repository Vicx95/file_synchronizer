#pragma once

#include "file_synchronizer.hpp"
#include "scanner.hpp"
#include "serializer.hpp"
#include "timer.hpp"
#include "streaming.hpp"

#include <chrono>
#include <filesystem>
#include <iostream> // TODO: delete
#include <string>

namespace fs = std::filesystem;

enum class ErrorCode
{
    SUCCESS = 0,
    FAIL = 1
};

class Model
{
public:
    Model();
    explicit Model(std::unique_ptr<i_Timer> syncTimer,
                   std::unique_ptr<i_FileSynchronizer> fileSynchronizer,
                   std::unique_ptr<i_Scanner> scanner,
                   std::unique_ptr<i_Serializer> serializer,
                   std::unique_ptr<Stream> stream) noexcept;

    virtual ~Model() = default;

    ErrorCode addDirectory(const std::string &dirName);
    ErrorCode removeDirectory(const std::string &dirName);
    ErrorCode removeFile(const std::string &dirName);
    ErrorCode getAllFilesInDir(const std::string &dirName, std::set<fs::path> &fileList);
    void createMainDir();

    void setIntervalTime(const std::string &strInterval);
    void setIntervalTime(std::chrono::duration<int64_t, std::milli> interval);
    void startSync();
    void stopSync();
    void forceSync();
    void readConfig();
    void saveConfig();
    void setupStreaming();
    fs::path getMainDirectoryPath();

private:
    bool validateForRemoval(std::string name);

    std::unique_ptr<i_Timer> m_syncTimer = nullptr;
    std::unique_ptr<i_FileSynchronizer> m_fileSynchronizer = nullptr;
    std::unique_ptr<i_Scanner> m_scanner = nullptr;
    std::unique_ptr<i_Serializer> m_serializer = nullptr;
    std::unique_ptr<Stream> m_stream = nullptr;


    std::chrono::duration<int64_t, std::milli> m_interval;
    const fs::path m_mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";
};

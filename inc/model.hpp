#pragma once

#include "file_synchronizer.hpp"
#include "scanner.hpp"
#include "serializer.hpp"
#include "timer.hpp"
#include "config_manager.hpp"
#include "file_streamer.hpp"

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
                   std::unique_ptr<i_ConfigManager> config_manager) noexcept;

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
    void setupNetwork();
    void startStreaming();
    void stopStreaming();
    fs::path getMainDirectoryPath();

private:
    bool validateForRemoval(std::string name);

    std::unique_ptr<i_Timer> m_syncTimer;
    std::unique_ptr<i_FileSynchronizer> m_fileSynchronizer;
    std::unique_ptr<i_Scanner> m_scanner;
    std::unique_ptr<i_ConfigManager> m_config_manager;

    std::chrono::duration<int64_t, std::milli> m_interval;
    const fs::path m_mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";

    std::vector<DirsAndFiles> m_fileStreamingConfig;
    std::vector<DirsAndNetworkParams> m_networkConfig;
    std::unordered_map<std::string, std::unique_ptr<i_FileStreamer>> m_file_streamers;

};

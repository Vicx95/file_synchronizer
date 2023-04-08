#pragma once

#include "file_synchronizer.hpp"
#include "scanner.hpp"
#include "serializer.hpp"
#include "timer.hpp"

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

class i_Model
{
public:
    virtual ~i_Model() = default;

    virtual void setIntervalTime(std::chrono::duration<int64_t, std::milli> interval) = 0;
    virtual ErrorCode addDirectory(const std::string &dirName) = 0;
    virtual ErrorCode removeDirectory(const std::string &dirName) = 0;
    virtual ErrorCode removeFile(std::istream &std_input) = 0;
    virtual void stopSync() = 0;
    virtual void forceSync() = 0;

};

class Model : public i_Model
{
public:
    Model();
    explicit Model(std::unique_ptr<i_Timer> syncTimer,
                   std::unique_ptr<i_FileSynchronizer> fileSynchronizer,
                   std::unique_ptr<i_Scanner> scanner,
                   std::unique_ptr<i_Serializer> serializer) noexcept;

    virtual ~Model() = default;

    ErrorCode addDirectory(const std::string &dirName) override;
    ErrorCode removeDirectory(const std::string &dirName) override;
    ErrorCode removeFile(std::istream &std_input) override;
    ErrorCode getAllFilesInDir(const std::string &dirName, std::set<fs::path> &fileList);
    void createMainDir();

    void setIntervalTime(const std::string &strInterval);
    void setIntervalTime(std::chrono::duration<int64_t, std::milli> interval) override;
    void startSync();
    void stopSync() override;
    void forceSync() override;
    void readConfig();
    void saveConfig();
    fs::path getMainDirectoryPath();

private:
    bool validateForRemoval(std::string name);

    std::unique_ptr<i_Timer> m_syncTimer = nullptr;
    std::unique_ptr<i_FileSynchronizer> m_fileSynchronizer = nullptr;
    std::unique_ptr<i_Scanner> m_scanner = nullptr;
    std::unique_ptr<i_Serializer> m_serializer = nullptr;

    std::chrono::duration<int64_t, std::milli> m_interval;
    const fs::path m_mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";
};

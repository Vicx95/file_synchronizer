#pragma once
#include "serializer.hpp"

class i_ConfigManager
{
public:
    ~i_ConfigManager() = default;
    virtual void loadFileConfig() = 0;
    virtual void saveFileConfig() = 0;
    virtual void loadStreamingConfig(std::vector<DirsAndFiles>& fileStreamingConfig)  = 0;
    virtual void loadNetworkConfig(std::vector<DirsAndNetworkParams>& networkConfig) = 0;
    virtual void printMachineNetwork(std::vector<DirsAndNetworkParams>& networkConfig) = 0;
    virtual void setMainDirectoryPath(const fs::path mainDirectoryPath) = 0;
};

class ConfigManager : public i_ConfigManager
{
public:
    
    void loadFileConfig() override;
    void saveFileConfig() override;
    void loadStreamingConfig(std::vector<DirsAndFiles>& fileStreamingConfig) override;
    void loadNetworkConfig(std::vector<DirsAndNetworkParams>& networkConfig) override;
    void printMachineNetwork(std::vector<DirsAndNetworkParams>& networkConfig) override;
    void setMainDirectoryPath(const fs::path mainDirectoryPath) override {m_mainDirectoryPath = mainDirectoryPath;};

private:
    std::unique_ptr<i_Serializer> m_serializer = std::make_unique<SerializerToJSON>();
    fs::path m_mainDirectoryPath;
};

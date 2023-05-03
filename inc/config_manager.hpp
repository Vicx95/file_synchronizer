#pragma once
#include "serializer.hpp"

class i_ConfigManager
{
public:
    ~i_ConfigManager() = default;
    virtual void loadFileConfig() = 0;
    virtual void saveFileConfig() = 0;
    virtual void loadStreamingConfig() = 0;
    virtual void loadNetworkConfig() = 0;
    virtual void printMachineNetwork(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> machinesNetwork) = 0;
};

class ConfigManager : public i_ConfigManager
{
public:
    ConfigManager(const fs::path mainDirectoryPath);
    void loadFileConfig() override;
    void saveFileConfig() override;
    void loadStreamingConfig() override;
    void loadNetworkConfig() override;
    void printMachineNetwork(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> machinesNetwork) override;

private:
    std::unique_ptr<i_Serializer> m_serializer = std::make_unique<SerializerToJSON>();
    const fs::path m_mainDirectoryPath;
};

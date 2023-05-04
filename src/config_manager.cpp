#include "..//inc/config_manager.hpp"

void ConfigManager::loadFileConfig()
{
    //fs::remove_all(m_mainDirectoryPath);
    //fs::create_directory(m_mainDirectoryPath);
    fs::path configPath = m_mainDirectoryPath / "config.json";
    //auto [dirs, files] = m_serializer->deserialize(configPath);
    std::filesystem::copy(m_mainDirectoryPath / "../configDirectory", m_mainDirectoryPath, std::filesystem::copy_options::recursive);
}

void ConfigManager::saveFileConfig()
{
    fs::remove_all(std::filesystem::current_path() / "../configDirectory");
    std::filesystem::copy(m_mainDirectoryPath, m_mainDirectoryPath / "../configDirectory", std::filesystem::copy_options::recursive);

    m_serializer->serialize();
}

void ConfigManager::loadStreamingConfig(std::vector<DirsAndFiles>& fileStreamingConfig)
{
    fs::path mainDirectoryPath = m_mainDirectoryPath;
    fs::path streamConfigPath = mainDirectoryPath / "streamConfig.json";
    fs::path tmpConfigPath = mainDirectoryPath / "tmpConfig";    
 
    fileStreamingConfig = m_serializer->deserialize(streamConfigPath);
    std::vector<DirsAndFiles> rejectedDirectoriesAndFiles = fileStreamingConfig;
    auto printFileName = [](auto &file) { std::cout << " " << file; };
    
    std::cout << "JSON streaming configuration: \n";
    for(const auto& [dir, files] : fileStreamingConfig)
    {
        std::cout << dir << ": ";
        std::for_each(files.cbegin(), files.cend(), printFileName);
        std::cout << "\n";   
    }
    std::cout << "List of applied streaming configuration: \n";
    auto dirNotExists = [&mainDirectoryPath](auto& directory) { return !fs::exists(mainDirectoryPath / directory.first); };
    fileStreamingConfig.erase(std::remove_if(fileStreamingConfig.begin(), fileStreamingConfig.end(), dirNotExists), fileStreamingConfig.end());
    auto fileNotExist = [&tmpConfigPath](auto &file) { return !fs::exists(tmpConfigPath / file); };
    for(auto& [dir, files] : fileStreamingConfig)
    {
        files.erase(std::remove_if(files.begin(), files.end(), fileNotExist), files.end());
        std::cout << dir << ": ";
        std::for_each(files.cbegin(), files.cend(), printFileName);
        std::cout << "\n";   
    }
    std::cout << "List of rejected files and directories: \n";
    auto dirExists = [&mainDirectoryPath](auto& directory) { return fs::exists(mainDirectoryPath / directory); };
    auto fileExist = [&tmpConfigPath](auto &file) { return fs::exists(tmpConfigPath / file); };
    for(auto& [dir, files] : rejectedDirectoriesAndFiles)
    {   
        if (dirExists(dir)){
            files.erase(std::remove_if(files.begin(), files.end(), fileExist), files.end());
        }
        std::cout << dir << ": ";
        std::for_each(files.cbegin(), files.cend(), printFileName);
        std::cout << "\n";   
    }
}

void ConfigManager::loadNetworkConfig(std::vector<DirsAndNetworkParams>& networkConfig)
{
    fs::path networkConfigPath = m_mainDirectoryPath / "networkConfig.json";
    fs::path mainDirectoryPath = m_mainDirectoryPath;
    
    networkConfig = m_serializer->deserializeNetwork(networkConfigPath);
    std::vector<DirsAndNetworkParams> rejectedNetworkConfig = networkConfig;

    std::cout << "JSON network configuration: \n";
    printMachineNetwork(networkConfig);

    auto dirNotExists = [&mainDirectoryPath](auto &directory) { return !fs::exists(mainDirectoryPath / directory.first); };
    auto dirExists = [&mainDirectoryPath](auto &directory) { return fs::exists(mainDirectoryPath / directory.first); };

    networkConfig.erase(std::remove_if(networkConfig.begin(), networkConfig.end(), dirNotExists), networkConfig.end());
    rejectedNetworkConfig.erase(std::remove_if(rejectedNetworkConfig.begin(), rejectedNetworkConfig.end(), dirExists), rejectedNetworkConfig.end());

    std::cout << "List of applied network configuration: \n";
    printMachineNetwork(networkConfig);

    std::cout << "List of rejected network configuration: \n";
    printMachineNetwork(rejectedNetworkConfig);
}

void ConfigManager::printMachineNetwork(std::vector<DirsAndNetworkParams>& networkConfig)
{
    for(const auto& [dir, network] : networkConfig)
    {
        std::cout << dir << ": " << network.first << " - " << network.second << "\n";
    }
}
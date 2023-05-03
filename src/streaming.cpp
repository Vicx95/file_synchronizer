#include "..//inc/streaming.hpp"


void Stream::loadStreaming()
{
    fs::path streamConfigPath = "/home/file_synchronizer/streamConfig.json";
    fs::path tmpConfigPath = "/home/file_synchronizer/tmpConfig";    
    fs::path mainDirectoryPath = "/home/file_synchronizer/mainDirectory";    
    
    std::vector<DirsAndFiles> machines = m_serializer->deserialize(streamConfigPath);
    std::vector<DirsAndFiles> rejectedMachines = machines;
    auto printFileName = [](auto &file) { std::cout << " " << file; };
    
    std::cout << "JSON streaming configuration: \n";
    for(const auto &machine : machines)
    {
        auto [dir, files] = machine;
        std::cout << dir << ": ";
        std::for_each(files.cbegin(), files.cend(), printFileName);
        std::cout << "\n";   
    }
    std::cout << "List of applied streaming configuration: \n";
        auto dirNotExists = [&mainDirectoryPath](auto &directory) { return !fs::exists(mainDirectoryPath / directory.first); };
        machines.erase(std::remove_if(machines.begin(), machines.end(), dirNotExists), machines.end());

    for(const auto &machine : machines)
    {
        auto [dir, files] = machine;
        auto fileNotExist = [&tmpConfigPath](auto &file) { return !fs::exists(tmpConfigPath / file); };
        files.erase(std::remove_if(files.begin(), files.end(), fileNotExist), files.end());
        std::cout << dir << ": ";
        std::for_each(files.cbegin(), files.cend(), printFileName);
        std::cout << "\n";   
    }
    std::cout << "List of rejected files and directories: \n";
    for(const auto &machine : rejectedMachines)
    {
        auto [dir, files] = machine;
        auto fileExist = [&tmpConfigPath](auto &file) { return fs::exists(tmpConfigPath / file); };
        if (!dirNotExists(machine)){
            files.erase(std::remove_if(files.begin(), files.end(), fileExist), files.end());
        }
        std::cout << dir << ": ";
        std::for_each(files.cbegin(), files.cend(), printFileName);
        std::cout << "\n";   
    }
}

void Stream::loadNetwork()
{
    fs::path networkConfigPath = "/home/file_synchronizer/networkConfig.json";
    fs::path mainDirectoryPath = "/home/file_synchronizer/mainDirectory";    
    
    std::vector<std::pair<std::string, std::pair<std::string, std::string>>> machinesNetwork = m_serializer->deserializeNetwork(networkConfigPath);
    std::vector<std::pair<std::string, std::pair<std::string, std::string>>> rejectedMachinesNetwork = machinesNetwork;

    std::cout << "JSON network configuration: \n";
    printMachineNetwork(machinesNetwork);

    auto dirNotExists = [&mainDirectoryPath](auto &directory) { return !fs::exists(mainDirectoryPath / directory.first); };
    auto dirExists = [&mainDirectoryPath](auto &directory) { return fs::exists(mainDirectoryPath / directory.first); };

    machinesNetwork.erase(std::remove_if(machinesNetwork.begin(), machinesNetwork.end(), dirNotExists), machinesNetwork.end());
    rejectedMachinesNetwork.erase(std::remove_if(rejectedMachinesNetwork.begin(), rejectedMachinesNetwork.end(), dirExists), rejectedMachinesNetwork.end());

    std::cout << "List of applied network configuration: \n";
    printMachineNetwork(machinesNetwork);

    std::cout << "List of rejected network configuration: \n";
    printMachineNetwork(rejectedMachinesNetwork);
}

void Stream::printMachineNetwork(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> machinesNetwork)
{
    for(const auto &machine : machinesNetwork)
    {
        auto [dir, network] = machine;
        std::cout << dir << ": " << network.first << " - " << network.second << "\n";
    }
}
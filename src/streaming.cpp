#include "..//inc/streaming.hpp"


void Stream::loadStreaming()
{
    fs::path streamConfigPath = "/home/file_synchronizer/streamConfig.json";
    fs::path tmpConfigPath = "/home/file_synchronizer/tmpConfig";    
    fs::path mainDirectoryPath = "/home/file_synchronizer/mainDirectory";    
    
    std::vector<DirsAndFiles> machines = m_serializer->deserialize(streamConfigPath);
    std::vector<DirsAndFiles> rejectedMachines;
    auto printFile = [](auto &file) { std::cout << " " << file; };

    for(auto &machine : machines)
    {
        auto [dir, files] = machine;
        std::cout << dir << ": ";
        std::for_each(files.cbegin(), files.cend(), printFile);
        std::cout << "\n";   
    }
    std::cout << "List of applied streaming configuration: \n";
        auto dirExist = [&mainDirectoryPath](auto &directory) { return !fs::exists(mainDirectoryPath / directory.first); };
        machines.erase(std::remove_if(machines.begin(), machines.end(), dirExist), machines.end());

    for(auto &machine : machines)
    {
        auto [dir, files] = machine;
        auto fileExist = [&tmpConfigPath](auto &file) { return !fs::exists(tmpConfigPath / file); };
        files.erase(std::remove_if(files.begin(), files.end(), fileExist), files.end());

        std::cout << dir << ": ";
        std::for_each(files.cbegin(), files.cend(), printFile);
        std::cout << "\n";   
    }


}

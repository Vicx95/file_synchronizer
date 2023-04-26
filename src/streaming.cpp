#include "..//inc/streaming.hpp"


void Stream::loadStreaming()
{
    fs::path streamConfigPath = "/file_synchronizer/streamConfig.json";
    fs::path tmpConfigPath = "/file_synchronizer/tmpConfig";    
    fs::path mainDirectoryPath = "/file_synchronizer/mainDirectory";    
    
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
        //std::copy_if(machines.begin(), machines.end(), std::back_inserter(rejectedMachines), dirNotExists);
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

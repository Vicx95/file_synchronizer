#include "..//inc/streaming.hpp"


void Stream::loadStreaming()
{
    fs::path streamConfigPath = "/home/file_synchronizer/streamConfig.json";    
    std::vector<DirsAndFiles> machines = m_serializer->deserialize(streamConfigPath);
   
    for(auto const &machine : machines)
    {
        auto [dir, files] = machine;
        std::cout << dir << ": ";
        auto printFile = [](auto &file) { std::cout << " " << file; };
        std::for_each(files.cbegin(), files.cend(), printFile);
        std::cout << "\n";   
    }
    
}

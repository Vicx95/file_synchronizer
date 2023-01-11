#include "..//inc/Serializer.hpp"
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <set>

void SerializerToJSON::serialize(){

    auto configOutputVector = m_scanner.scanForChangedDirs(std::filesystem::current_path() / "../mainDirectory");
    json config;
    std::set<std::pair<std::string, std::set<std::string>>> DirAndFiles;
    std::string dir;
    std::set<std::string> files;
    std::vector<PathAndTime>::iterator it = configOutputVector.begin();

    for(const auto& output : configOutputVector){
        auto nx = std::next(it, 1);
        PathAndTime nextVlue = *nx;
        if(fs::is_directory(output.first)){
            dir = output.first.filename();
        }

        if(fs::is_regular_file(output.first))
            files.insert(output.first.filename());
        
        if(fs::is_directory(nextVlue.first)){
            DirAndFiles.insert(std::make_pair(dir, files));
            files.clear();
        }
        it++;
    }
        DirAndFiles.insert(std::make_pair(dir, files));

/*  DEBUG
    for(auto el : DirAndFiles){
        std::cout << "Dir: " << el.first << std::endl;
        for(auto x : el.second){
            std::cout << "file: " << x << std::endl;
        }
    }
 */   
   for(const auto& dirAndFile : DirAndFiles){
        config[dirAndFile.first] = dirAndFile.second;
    }

    std::ofstream file("../config.json");
    config.dump();
    file << std::setw(4) << config;
}

void SerializerToJSON::deserialize(){
    std::ifstream config("../config.json");
    json dirAndFilesJSON = json::parse(config);
    std::cout << dirAndFilesJSON << std::endl; 
    std::vector<std::string> Dirs;
    std::vector<std::vector<std::string>> files;

    for(auto& element : dirAndFilesJSON) {
        std::cout << element << '\n';
    }

    for (json::iterator it = dirAndFilesJSON.begin(); it != dirAndFilesJSON.end(); ++it) {
        std::vector<std::string> filesInDir;
        std::cout << it.key() << " : " << it.value() << "\n";
        Dirs.push_back(it.key());
        for(auto fileInDir : it.value()){
            std::cout << "el: " << fileInDir << std::endl;
            filesInDir.push_back(fileInDir);
        }
        files.push_back(filesInDir);
    }

/* DEBUG
    for(auto filesInDir : files){
        for(auto file : filesInDir){
            std::cout << "File: " << file << std::endl;
        }
        std::cout << "\n";
    }
*/
}
/*
void SerializerToJSON::readConfig()
{
    fs::remove_all(path);

    auto [dirs, files] = deserialize();

    for(dir : dirs){
        fs::create_directory(dir);
    
    }

}
*/
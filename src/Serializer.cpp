#include "..//inc/Serializer.hpp"
#include <vector>
#include <fstream>
#include <sstream>
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

DirsAndFiles SerializerToJSON::deserialize(){
    std::ifstream config("../config.json");
    json dirAndFilesJSON = json::parse(config);
    //std::cout << dirAndFilesJSON << std::endl; 
    std::vector<std::string> Dirs;
    std::vector<std::vector<std::string>> files;

    for (json::iterator it = dirAndFilesJSON.begin(); it != dirAndFilesJSON.end(); ++it) {
        std::vector<std::string> filesInDir;
        Dirs.push_back(it.key());
        for(auto fileInDir : it.value()){
            filesInDir.push_back(fileInDir);
        }
        files.push_back(filesInDir);
    }
return {std::make_pair(Dirs, files)};
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
    fs::remove_all(std::filesystem::current_path() / "../mainDirectory");
    fs::create_directory(std::filesystem::current_path() / "../mainDirectory");
    fs::current_path(std::filesystem::current_path() / "../mainDirectory");  
    auto [dirs, files] = deserialize();

    for(auto dir : dirs){
        fs::create_directory(std::filesystem::current_path() / dir);
        std::filesystem::copy(std::filesystem::current_path() / "../configDirectory" / dir, std::filesystem::current_path() / "../mainDirectory" / dir, std::filesystem::copy_options::recursive);
        std::cout << dir << std::endl;
    
    }

}
void SerializerToJSON::saveConfig()
{
    std::filesystem::copy(std::filesystem::current_path() / "../mainDirectory", std::filesystem::current_path() / "../configDirectory", std::filesystem::copy_options::recursive);
    serialize();
}
*/


void SerializerToTxt::serialize(){
    auto configOutputVector = m_scanner.scanForChangedDirs(std::filesystem::current_path() / "../mainDirectory");
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

    std::ofstream config("../config.txt", std::ofstream::out);

    if (config.is_open())
    {
        for(const auto& dirAndFile : DirAndFiles){
             config << dirAndFile.first <<  " : " ;
             for(auto file : dirAndFile.second){
                config << file << ", ";
             }
             config << "\n";
        }
        config.close();
    }
}

DirsAndFiles SerializerToTxt::deserialize(){
    std::ifstream config("../config.txt");
    std::string dirAndFilesTXT;
    std::vector<std::string> Dirs;
    std::vector<std::vector<std::string>> files;

    while(getline(config, dirAndFilesTXT)) {
  
        //std::cout << dirAndFilesTXT << std::endl;
        size_t pos = dirAndFilesTXT.find(":");
        //size_t comma = dirAndFilesTXT.find(",");
        std::string dir = dirAndFilesTXT.substr(0, pos-1);
        std::string fileTxt = dirAndFilesTXT.substr(pos+2);
        Dirs.push_back(dir);

        std::vector<std::string> fileInDir;
        std::stringstream filesSS(fileTxt);
        while(filesSS.good()) {
            std::string file;
            getline(filesSS, file, ','); //get first string delimited by comma
            fileInDir.push_back(file);
        }
        files.push_back(fileInDir);
    }


    config.close();

    for(auto dir : Dirs)
        std::cout << "Dir: " << dir << std::endl;

    for(auto fileInDir : files)
        for(auto file : fileInDir)
            std::cout << "file: " << file << std::endl;    
    /*
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
    */
   return {};
}
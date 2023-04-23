#include "..//inc/serializer.hpp"

void SerializerToJSON::serialize()
{

    auto configOutputVector = m_scanner.getRecentScanning();
    json config;
    std::set<std::pair<std::string, std::set<std::string>>> DirAndFiles;
    std::string dir;
    std::set<std::string> files;

    for (const auto &output : configOutputVector)
    {
        if (fs::is_directory(output.first))
        {
            dir = output.first.filename();
        }

        if (fs::is_regular_file(output.first))
        {
            files.insert(output.first.filename());
        }
        DirAndFiles.insert(std::make_pair(dir, files));
        files.clear();
    }

    for (const auto &dirAndFile : DirAndFiles)
    {
        config[dirAndFile.first] = dirAndFile.second;
    }

    std::ofstream file("../config.json");
    config.dump();
    file << std::setw(4) << config;
}

std::vector<DirsAndFiles> SerializerToJSON::deserialize(const std::string &filePath)
{
    std::ifstream config(filePath);
    json dirAndFilesJSON = json::parse(config);
    std::vector<std::string> Dirs;
    std::vector<std::vector<std::string>> files;
    std::vector<DirsAndFiles> result;

    for (json::iterator it = dirAndFilesJSON.begin(); it != dirAndFilesJSON.end(); ++it)
    {
        std::vector<std::string> filesInDir;
        Dirs.push_back(it.key());
        for (auto fileInDir : it.value())
        {
            filesInDir.push_back(fileInDir);
        }
        files.push_back(filesInDir);
        result.push_back(std::make_pair(it.key(), filesInDir));
    }
    return result;

}

void SerializerToTxt::serialize()
{
    auto configOutputVector = m_scanner.getRecentScanning();
    std::set<std::pair<std::string, std::set<std::string>>> DirAndFiles;
    std::string dir;
    std::set<std::string> files;

    for (const auto &output : configOutputVector)
    {
        if (fs::is_directory(output.first))
        {
            dir = output.first.filename();
        }

        if (fs::is_regular_file(output.first))
        {
            files.insert(output.first.filename());
        }
        DirAndFiles.insert(std::make_pair(dir, files));
        files.clear();
    }

    std::ofstream config("../config.txt", std::ofstream::out);

    if (config.is_open())
    {
        for (const auto &dirAndFile : DirAndFiles)
        {
            config << dirAndFile.first << " : ";
            for (auto file : dirAndFile.second)
            {
                config << file << ", ";
            }
            config << "\n";
        }
        config.close();
    }
}

std::vector<DirsAndFiles> SerializerToTxt::deserialize(const std::string &filePath)
{
    std::ifstream config(filePath);
    std::string dirAndFilesTXT;
    std::vector<std::string> Dirs;
    std::vector<std::vector<std::string>> files;

    while (getline(config, dirAndFilesTXT))
    {

        // std::cout << dirAndFilesTXT << std::endl;
        size_t pos = dirAndFilesTXT.find(":");
        // size_t comma = dirAndFilesTXT.find(",");
        std::string dir = dirAndFilesTXT.substr(0, pos - 1);
        std::string fileTxt = dirAndFilesTXT.substr(pos + 2);
        Dirs.push_back(dir);

        std::vector<std::string> fileInDir;
        std::stringstream filesSS(fileTxt);
        while (filesSS.good())
        {
            std::string file;
            getline(filesSS, file, ','); // get first string delimited by comma
            fileInDir.push_back(file);
        }
        files.push_back(fileInDir);
    }

    config.close();

    for (auto dir : Dirs)
        std::cout << "Dir: " << dir << std::endl;

    for (auto fileInDir : files)
        for (auto file : fileInDir)
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
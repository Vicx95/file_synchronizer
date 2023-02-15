#pragma once

#include "../inc/nlohmann/json.hpp"
#include "../inc/scanner.hpp"
#include <set>

using json = nlohmann::json;
namespace fs = std::filesystem;
using DirsAndFiles = std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>>;

class Serializer {


public:

    virtual void serialize() = 0;
    virtual DirsAndFiles deserialize() = 0;
    virtual ~Serializer() {}
};

class SerializerToJSON : public Serializer{
    Scanner m_scanner;
public:
    void serialize();
    DirsAndFiles deserialize();

};

class SerializerToTxt : public Serializer{
    Scanner m_scanner;
public:
    void serialize();
    DirsAndFiles deserialize();
};
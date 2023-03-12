#pragma once

#include "nlohmann/json.hpp"
#include "scanner.hpp"

#include <fstream>
#include <iostream> // TODO: delete
#include <set>
#include <vector>

namespace fs = std::filesystem;

using json = nlohmann::json;
using DirsAndFiles = std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>>;

class i_Serializer
{
public:
    virtual void serialize() = 0;
    virtual DirsAndFiles deserialize() = 0;
};

class SerializerToJSON : public i_Serializer
{
    Scanner m_scanner;

public:
    virtual ~SerializerToJSON();

    void serialize() override;
    DirsAndFiles deserialize() override;
};

class SerializerToTxt : public i_Serializer
{
    Scanner m_scanner;

public:
    virtual ~SerializerToTxt();

    void serialize() override;
    DirsAndFiles deserialize() override;
};
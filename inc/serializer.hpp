#pragma once

#include "nlohmann/json.hpp"
#include "scanner.hpp"

#include <fstream>
#include <iostream> // TODO: delete
#include <set>
#include <vector>

namespace fs = std::filesystem;

using json = nlohmann::json;
using DirsAndFiles = std::pair<std::string, std::vector<std::string>>;

class i_Serializer
{
public:
    virtual ~i_Serializer() = default;

    virtual void serialize() = 0;
    virtual std::vector<DirsAndFiles> deserialize(const std::string &filePath) = 0;
};

class SerializerToJSON : public i_Serializer
{
    Scanner m_scanner;

public:
    void serialize() override;
    std::vector<DirsAndFiles> deserialize(const std::string &filePath) override;
};

class SerializerToTxt : public i_Serializer
{
    Scanner m_scanner;

public:
    void serialize() override;
    std::vector<DirsAndFiles> deserialize(const std::string &filePath) override;
};
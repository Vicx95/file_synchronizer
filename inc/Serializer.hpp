#include "../inc/nlohmann/json.hpp"
#include "../inc/Scanner.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;


class Serializer {


public:

    virtual void serialize() = 0;
    virtual void deserialize() = 0;
};

class SerializerToJSON : public Serializer{
Scanner m_scanner;
public:
    void serialize();
    void deserialize();
    void readConfig();
    void saveConfig();
};

class SerializerToTxt : public Serializer{
public:
    void serialize();
    void deserialize();
};
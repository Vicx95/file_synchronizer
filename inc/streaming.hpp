#pragma once
#include "serializer.hpp"

class Stream
{

public:

    void loadStreaming();
    void loadNetwork();
    void printMachineNetwork(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> machinesNetwork);



private:
    std::unique_ptr<i_Serializer> m_serializer = std::make_unique<SerializerToJSON>();

};

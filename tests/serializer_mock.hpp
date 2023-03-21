#pragma once

#include "..//inc/serializer.hpp"
#include "gmock/gmock.h" // Brings in gMock.

class MockSerializer : public i_Serializer
{
public:
    MOCK_METHOD(void, serialize, (), (override));
    MOCK_METHOD(DirsAndFiles, deserialize, (), (override));
};
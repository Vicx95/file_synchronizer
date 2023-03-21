#pragma once

#include "..//inc/timer.hpp"
#include "gmock/gmock.h" // Brings in gMock.

class MockTimer : public i_Timer
{
public:
    MOCK_METHOD(void, start, (const Interval &interval, const Callback &timeoutCallback), (override));
    MOCK_METHOD(void, stop, (), (override));
};
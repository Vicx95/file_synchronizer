#include "..//inc/file_synchronizer.hpp"
#include "gmock/gmock.h" // Brings in gMock.

class MockSynchronizer : public i_FileSynchronizer
{
public:
    MOCK_METHOD(void, synchronizeAdded, (const std::vector<PathTimePair_t> &addedFiles), (override));
    MOCK_METHOD(void, synchronizeRemoved, (const std::vector<PathTimePair_t> &removedFiles), (override));
};
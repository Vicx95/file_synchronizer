#include "..//inc/Scanner.hpp"
#include "gmock/gmock.h" // Brings in gMock.

class MockScanner : public i_Scanner
{
public:
    MOCK_METHOD(void, scan, (const fs::path &dirPath), (override));
    MOCK_METHOD(void, scanForChangedDirs, (const fs::path &dirPath), (override));
    MOCK_METHOD((std::pair<std::vector<PathTimePair_t>, std::vector<PathTimePair_t>>), comparePreviousAndRecentScanning, (), (override));
};
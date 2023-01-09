#pragma once
#include <chrono>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
using PathAndTime = std::pair<fs::path, fs::file_time_type>;

class Scanner
{
public:
    Scanner();
    void synchronize(std::pair<std::vector<PathAndTime>, std::vector<PathAndTime>> AddedAndRemovedFiles);
    void scan(fs::path dirPath);
    std::vector<PathAndTime> scanForChangedDirs(const fs::path &dirPath);
    void printRecentScanResult();
    void printPreviousScanResult();
    std::string convertDateTimeToString(fs::file_time_type ftime);
    std::pair<std::vector<PathAndTime>, std::vector<PathAndTime>> comparePreviousAndRecentScanning(std::vector<PathAndTime> previous, std::vector<PathAndTime> last);

private:
    std::vector<PathAndTime> m_recentScanning;
    std::vector<PathAndTime> m_previousScanning;
};

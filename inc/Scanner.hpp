#pragma once
#include <chrono>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
using PathTimePair_t = std::pair<fs::path, fs::file_time_type>;

class i_Scanner
{
public:
    virtual ~i_Scanner(){};
    virtual void scan(const fs::path &dirPath) = 0;
    virtual void scanForChangedDirs(const fs::path &dirPath) = 0;
    virtual std::pair<std::vector<PathTimePair_t>, std::vector<PathTimePair_t>> comparePreviousAndRecentScanning() = 0;
};

class Scanner : public i_Scanner
{
public:
    Scanner();

    void scan(const fs::path &dirPath) override;
    void scanForChangedDirs(const fs::path &dirPath) override;
    std::pair<std::vector<PathTimePair_t>, std::vector<PathTimePair_t>> comparePreviousAndRecentScanning() override;

    void printRecentScanResult();
    void printPreviousScanResult();

private:
    std::string convertDateTimeToString(fs::file_time_type ftime);

    std::vector<PathTimePair_t> m_recentScanning;
    std::vector<PathTimePair_t> m_previousScanning;
};

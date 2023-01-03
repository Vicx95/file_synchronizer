#pragma once
#include <chrono>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
using PathAndTime = std::pair<fs::path, fs::file_time_type>;

class Scanner {
public:
    Scanner();
    void synchronize(std::pair<std::vector<PathAndTime>, std::vector<PathAndTime>> AddedAndRemovedFiles);
    void scan(fs::path dirPath);
    std::vector<PathAndTime> scanning(fs::path dirPath);
    void printLastScanning();
    void printPreviousLastScanning();
    std::string convertDateTimeToString(fs::file_time_type ftime);
    std::pair<std::vector<PathAndTime>, std::vector<PathAndTime>>  comparingPreviousAndLastScanning(std::vector<PathAndTime> previous, std::vector<PathAndTime> last);

private:
    std::vector<PathAndTime> m_lastScanning;
    std::vector<PathAndTime> m_previousLastScanning;
    // auto time_compare = [](const fs::path& lhs, const fs::path& rhs) {
    //     const auto lhs_time = std::filesystem::last_write_time(lhs);
    //     const auto rhs_time = std::filesystem::last_write_time(rhs);
    //     return lhs_time == rhs_time ? lhs < rhs : lhs_time < rhs_time;
    //  };
    //std::function<bool(fs::file_time_type, fs::file_time_type)> time_comparator = [](const fs::file_time_type& lhs, const fs::file_time_type& rhs) {
    //    return lhs == rhs ? lhs < rhs : lhs < rhs;
    //};

    //bool time_compare(PathAndTime lhs, PathAndTime rhs){
    //    return lhs.first == rhs.first && lhs.second < rhs.second;  
    //}
};

#include "..//inc/scanner.hpp"
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

Scanner::Scanner()
{
    m_recentScanning.reserve(1024);
    this->scanForChangedDirs(std::filesystem::current_path() / "../mainDirectory");
}

void Scanner::scanForChangedDirs(const fs::path &dirPath)
{
    if (!fs::exists(dirPath))
    {
        fs::create_directory(dirPath);
    }

    for (auto const &dirEntry : fs::recursive_directory_iterator(dirPath))
    {
        m_recentScanning.push_back({dirEntry.path(), dirEntry.last_write_time()});
    }
}

void Scanner::scan(const fs::path &dirPath)
{
    m_previousScanning = m_recentScanning;
    m_recentScanning.clear();
    this->scanForChangedDirs(dirPath);
}

void Scanner::printRecentScanResult()
{
    for (auto const &file : m_recentScanning)
    {
        std::cout << "File name: " << file.first.filename() << "\n";
        std::cout << "Date: " << convertDateTimeToString(file.second) << "\n";
    }
}

void Scanner::printPreviousScanResult()
{
    for (auto const &file : m_previousScanning)
    {
        std::cout << "File name: " << file.first.filename() << "\n";
        std::cout << "Date: " << convertDateTimeToString(file.second) << "\n";
    }
}

std::string Scanner::convertDateTimeToString(fs::file_time_type ftime)
{
    // std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
    std::time_t cftime = std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(fs::file_time_type::clock::time_point(ftime)));

    std::stringstream ss;
    ss << std::put_time(std::localtime(&cftime), "%Y-%m-%d %X");
    return ss.str();
}

std::pair<std::vector<PathTimePair_t>, std::vector<PathTimePair_t>> Scanner::comparePreviousAndRecentScanning()
{
    std::set<PathTimePair_t> sortedPrevious, sortedLast;
    // Sorting
    for (auto const &prev : m_previousScanning)
    {
        sortedPrevious.insert(prev);
    }
    for (auto const &next : m_recentScanning)
    {
        sortedLast.insert(next);
    }
    // Added file scanning
    std::vector<PathTimePair_t> Added;
    std::set_difference(sortedLast.begin(), sortedLast.end(), sortedPrevious.begin(), sortedPrevious.end(),
                        std::inserter(Added, Added.begin()));

    std::cout << "result of comparing Added file: \n";
    for (auto el : Added)
    {
        if (fs::is_regular_file(el.first))
        {
            std::cout << "File name: " << el.first.filename() << "\n";
            std::cout << "Date: " << convertDateTimeToString(el.second) << "\n";
        }
    }
    // Removed file scanning
    std::vector<PathTimePair_t> Removed;
    std::set_difference(sortedPrevious.begin(), sortedPrevious.end(), sortedLast.begin(), sortedLast.end(),
                        std::inserter(Removed, Removed.begin()), [](PathTimePair_t lhs, PathTimePair_t rhs) { return lhs < rhs; });
    std::cout << "result of comparing Removed file: \n";
    for (auto el : Removed)
    {
        if (!fs::is_directory(el.first))
        {
            std::cout << "File name: " << el.first.filename() << "\n";
            std::cout << "Date: " << convertDateTimeToString(el.second) << "\n";
        }
    }

    return {std::make_pair(Added, Removed)};
}

std::vector<PathTimePair_t> Scanner::getRecentScanning()
{
    return m_recentScanning;
}

std::vector<PathTimePair_t> Scanner::getPreviousScanning()
{
    return m_previousScanning;
}
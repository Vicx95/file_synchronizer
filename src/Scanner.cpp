#include "..//inc/Scanner.hpp"
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

Scanner::Scanner()
{
    m_recentScanning = this->scanForChangedDirs(std::filesystem::current_path() / "../mainDirectory");
}

std::vector<PathAndTime> Scanner::scanForChangedDirs(const fs::path &dirPath)
{
    if(!fs::exists(dirPath)){
        fs::create_directory(dirPath);
    }
    std::vector<PathAndTime> result;
    result.reserve(1024);

    for (auto const &dirEntry : fs::recursive_directory_iterator(dirPath))
    {
        result.push_back({dirEntry.path(), dirEntry.last_write_time()});
    }

    result.shrink_to_fit();
    return result;
}

void Scanner::scan(fs::path dirPath)
{
    m_previousScanning = m_recentScanning;
    m_recentScanning = this->scanForChangedDirs(dirPath);
    auto outputComparing = comparePreviousAndRecentScanning(m_previousScanning, m_recentScanning);
    synchronize(outputComparing);
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
    std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
    //std::time_t cftime = std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(fs::file_time_type::clock::time_point(ftime)));

    std::stringstream ss;
    ss << std::put_time(std::localtime(&cftime), "%Y-%m-%d %X");
    return ss.str();
}

std::pair<std::vector<PathAndTime>, std::vector<PathAndTime>> Scanner::comparePreviousAndRecentScanning(std::vector<PathAndTime> previous, std::vector<PathAndTime> last)
{
    std::set<PathAndTime> sortedPrevious, sortedLast;
    // Sorting
    for (auto const &prev : previous)
    {
        sortedPrevious.insert(prev);
    }
    for (auto const &next : last)
    {
        sortedLast.insert(next);
    }
    // Added file scanning
    std::vector<PathAndTime> Added;
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
    std::vector<PathAndTime> Removed;
    std::set_difference(sortedPrevious.begin(), sortedPrevious.end(), sortedLast.begin(), sortedLast.end(),
                        std::inserter(Removed, Removed.begin()), [](PathAndTime lhs, PathAndTime rhs) { return lhs < rhs; });
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

void Scanner::synchronize(std::pair<std::vector<PathAndTime>, std::vector<PathAndTime>> AddedAndRemovedFiles)
{
    for (auto el : AddedAndRemovedFiles.first)
    {
        if (fs::is_regular_file(el.first))
        {
            for (auto dir : fs::directory_iterator(el.first.parent_path().parent_path()))
                try
                {
                    if (el.first != dir / el.first.filename())
                        fs::copy_file(el.first, dir / el.first.filename(), fs::copy_options::update_existing);
                }
                catch (std::exception &e)
                {
                    std::cout << e.what();
                }
        }
    }
    m_recentScanning = this->scanForChangedDirs(std::filesystem::current_path() / "../mainDirectory");

    for (auto el : AddedAndRemovedFiles.second)
    {
        for (auto dir : fs::directory_iterator(el.first.parent_path().parent_path()))
            try
            {
                if (el.first != dir / el.first.filename() && !(fs::exists(el.first)))
                {
                    fs::remove(dir / el.first.filename());
                }
            }
            catch (std::exception &e)
            {
                std::cout << e.what();
            }
    }

    m_recentScanning = this->scanForChangedDirs(std::filesystem::current_path() / "../mainDirectory");
}

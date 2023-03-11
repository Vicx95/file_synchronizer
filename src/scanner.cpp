#include "..//inc/scanner.hpp"
#include "..//inc/logger.hpp"

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
    std::vector<PathTimePair_t> added;

    std::set_difference(sortedLast.begin(), sortedLast.end(), sortedPrevious.begin(), sortedPrevious.end(),
                        std::inserter(added, added.begin()));
    if (!added.empty())
    {
        for (auto el : added)
        {
            if (fs::is_regular_file(el.first))
            {
                std::string fileName = el.first.filename();
                LOG_INFO("Added file: " + fileName);
            }
        }
    }

    // Removed file scanning
    std::vector<PathTimePair_t> removed;
    std::set_difference(sortedPrevious.begin(), sortedPrevious.end(), sortedLast.begin(), sortedLast.end(),
                        std::inserter(removed, removed.begin()), [](PathTimePair_t lhs, PathTimePair_t rhs) { return lhs < rhs; });
    if (!removed.empty())
    {
        for (auto el : removed)
        {
            if (fs::is_regular_file(el.first))
            {
                std::string fileName = el.first.filename();
                LOG_INFO("Removed file: " + fileName);
            }
        }
    }

    return {std::make_pair(added, removed)};
}

std::vector<PathTimePair_t> Scanner::getRecentScanning()
{
    return m_recentScanning;
}

std::vector<PathTimePair_t> Scanner::getPreviousScanning()
{
    return m_previousScanning;
}

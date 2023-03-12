#include "..//inc/scanner.hpp"
#include "..//inc/logger.hpp"

Scanner::Scanner()
{
    m_recentScanning.reserve(1024);
    this->scanForChangedDirs(std::filesystem::current_path() / "../mainDirectory");
}

Scanner::~Scanner()
{
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

AddedRemovedVectorPair_t Scanner::comparePreviousAndRecentScanning()
{
    std::vector<PathTimePair_t> sortedPrevious = m_previousScanning;
    std::vector<PathTimePair_t> sortedLast = m_recentScanning;

    std::ranges::sort(sortedPrevious);
    std::ranges::sort(sortedLast);

    // Added file scanning
    std::vector<PathTimePair_t> added;
    std::set_difference(sortedLast.begin(), sortedLast.end(), sortedPrevious.begin(), sortedPrevious.end(),
                        std::inserter(added, added.begin()));

    // Removed file scanning
    std::vector<PathTimePair_t> removed;
    std::set_difference(sortedPrevious.begin(), sortedPrevious.end(), sortedLast.begin(), sortedLast.end(),
                        std::inserter(removed, removed.begin()), [](PathTimePair_t lhs, PathTimePair_t rhs) { return lhs < rhs; });

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

void Scanner::logScanResult(const AddedRemovedVectorPair_t &vecPair)
{
    std::string msg("Added file: ");

    auto log = [&msg](const PathTimePair_t &p) {    
        if (fs::is_regular_file(p.first))
        {
            std::string filename = p.first.filename();
            msg.append(filename);
            LOG_INFO(msg);
        } };

    std::ranges::for_each(vecPair.first, log);
    msg = "Removed file: ";
    std::ranges::for_each(vecPair.second, log);
}
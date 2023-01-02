#include <filesystem>
#include <iostream>
#include "..//inc/Scanner.hpp"
#include <string>
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream> 
#include <set>


Scanner::Scanner() {
    m_lastScanning = scanning(std::filesystem::current_path() / "../mainDirectory");
}

std::vector<std::pair<fs::path, fs::file_time_type>> Scanner::scanning(fs::path dirPath){
    std::vector<std::pair<fs::path, fs::file_time_type>> result;
    for (auto const& dirEntry : fs::recursive_directory_iterator(dirPath)) {
        result.push_back({dirEntry.path(), dirEntry.last_write_time() });
    }
    return result;
}

void Scanner::scan(fs::path dirPath){
    m_previousLastScanning = m_lastScanning;
    m_lastScanning = scanning(dirPath);
    auto [addedFiles, removedFiles] = comparingPreviousAndLastScanning(m_previousLastScanning, m_lastScanning);
    /*
    if(resultComparing == 0)
        return m_lastScanning;
    else{

    }
    */
}

void Scanner::printLastScanning(){
    for(auto const& file : m_lastScanning){
        std::cout << "File name: " << file.first.filename() << "\n";
        std::cout << "Date: " <<  convertDateTimeToString(file.second) << "\n";
    }
}

void Scanner::printPreviousLastScanning(){
    for(auto const& file : m_previousLastScanning){
        std::cout << "File name: " << file.first.filename() << "\n";
        std::cout << "Date: " <<  convertDateTimeToString(file.second) << "\n";
    }
}

std::string Scanner::convertDateTimeToString(fs::file_time_type ftime){

    std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&cftime), "%Y-%m-%d %X");
    return ss.str();
}

std::pair<std::vector<PathAndTime>, std::vector<PathAndTime>> Scanner::comparingPreviousAndLastScanning(std::vector<PathAndTime> previous, std::vector<PathAndTime> last){
    std::set<PathAndTime> sortedPrevious, sortedLast;
    //Sorting
    for (auto const& prev : previous) {
            sortedPrevious.insert(prev);
        }
    for (auto const& next : last) {
            sortedLast.insert(next);
        }
    //Added file scanning
    std::vector<PathAndTime> Added;
    std::set_difference(sortedLast.begin(), sortedLast.end(), sortedPrevious.begin(), sortedPrevious.end(),
                        std::inserter(Added, Added.begin()));

    std::cout << "result of comparing Added file: \n";
    for(auto el : Added)
    {
        if(fs::is_regular_file(el.first)){
            std::cout << "File name: " << el.first.filename() << "\n";
            std::cout << "Date: " <<  convertDateTimeToString(el.second) << "\n";
        }
    }
    //Removed file scanning 
    std::vector<PathAndTime> Removed; 
    std::set_difference(sortedPrevious.begin(), sortedPrevious.end(), sortedLast.begin(), sortedLast.end(),
                        std::inserter(Removed, Removed.begin()), [](PathAndTime lhs, PathAndTime rhs){return lhs < rhs;}  );
    std::cout << "result of comparing Removed file: \n";
    for(auto el : Removed)
    {
        std::cout << el.first <<" debug 1 \n";
        if(fs::is_regular_file(el.first)){
            std::cout << "debug 2 \n";
            std::cout << "File name: " << el.first.filename() << "\n";
            std::cout << "Date: " <<  convertDateTimeToString(el.second) << "\n";
        }
    }

    return {std::make_pair(Added, Removed)};
}
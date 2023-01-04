#include "..//inc/FileSynchronizer.hpp"
#include <iostream>

FileSynchronizer::FileSynchronizer(){
    std::filesystem::path mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";

    m_timer.start(std::chrono::milliseconds(15000), [this, mainDirectoryPath] {
        m_scanner.scan(mainDirectoryPath);
        std::puts("Synchronizuje!");
    });
    
   // std::this_thread::sleep_for(std::chrono::seconds(10));
}

void FileSynchronizer::setTimeInterval(){
    std::cout << "Timer interval set to: \n";
}
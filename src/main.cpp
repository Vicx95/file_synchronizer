#include <iostream>
#include "..//inc/Scanner.hpp"
#include <string>
#include <sstream> 
#include <chrono>  // chrono::system_clock
#include <ctime> 


int main() {
    std::cout << "Hello from app!\n";
    Scanner scanner;
    std::filesystem::path mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";
    scanner.scan(mainDirectoryPath);
    //scanner.printLastScanning();
    //scanner.printPreviousLastScanning();
    uint32_t num;
    std::cin >> num;
    scanner.scan(mainDirectoryPath);

}
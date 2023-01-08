#include "..//inc/FileSynchronizer.hpp"
#include "..//inc/Scanner.hpp"
#include "..//inc/controller.hpp"
#include "..//inc/model.hpp"
#include "..//inc/timer.hpp"
#include "..//inc/view.hpp"

#include <chrono> // chrono::system_clock
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    Timer tm;
    View v;
    FileSynchronizer sync;
    Model m(&tm, &sync);
    Controller c(&v, &m);
    v.setListener(&c);
    v.run();

    /*     std::cout << "Hello from app!\n";
        FileSynchronizer synchronizer;
        // Scanner scanner;
        //   std::filesystem::path mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";
        // scanner.scan(mainDirectoryPath);
        // scanner.printLastScanning();
        // scanner.printPreviousLastScanning();
        uint32_t num;
        std::cin >> num; // input number to exit program.
        //  scanner.scan(mainDirectoryPath); */
}

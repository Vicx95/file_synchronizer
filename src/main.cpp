#include "..//inc/controller.hpp"
#include "..//inc/file_synchronizer.hpp"
#include "..//inc/logger.hpp"
#include "..//inc/model.hpp"
#include "..//inc/scanner.hpp"
#include "..//inc/serializer.hpp"
#include "..//inc/timer.hpp"
#include "..//inc/view.hpp"

#include <chrono> // chrono::system_clock
#include <ctime>
#include <future>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    LOG_INFO("Synchronizer starting");

    try
    {
        Timer tm;
        View v;
        Scanner scanner;
        FileSynchronizer sync;

        Model m(&tm, &sync, &scanner);
        Controller c(&v, &m);
        c.run();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
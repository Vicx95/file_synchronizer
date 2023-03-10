#include "..//inc/controller.hpp"
#include "..//inc/file_synchronizer.hpp"
#include "..//inc/logger.hpp"
#include "..//inc/model.hpp"
#include "..//inc/scanner.hpp"
#include "..//inc/serializer.hpp"
#include "..//inc/timer.hpp"
#include "..//inc/view.hpp"
#include "../inc/thread_pool_provider.hpp"

#include <chrono> // chrono::system_clock
#include <ctime>
#include <future>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    LOG_INFO("Synchronizer starting");

    Timer tm;
    View v;
    Scanner scanner;
    FileSynchronizer sync;

    auto future = ThreadPoolProvider::instance().getThreadPool()->submit([]() { return "Chuck Testa"; });
    std::cout << future.get() << '\n';

    Model m(&tm, &sync, &scanner);
    Controller c(&v, &m);
    v.setListener(&c);
    v.run();
}
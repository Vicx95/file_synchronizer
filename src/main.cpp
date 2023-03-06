#include "..//inc/FileSynchronizer.hpp"
#include "..//inc/Scanner.hpp"
#include "..//inc/Serializer.hpp"
#include "..//inc/ThreadPool.hpp"
#include "..//inc/controller.hpp"
#include "..//inc/model.hpp"
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

    Timer tm;
    View v;
    Scanner scanner;
    FileSynchronizer sync;

    ThreadPool tp;
    auto future = tp.submit([]() { return "Chuck Testa"; });

    std::cout << future.get() << '\n';

    Model m(&tm, &sync, &scanner);
    Controller c(&v, &m);
    v.setListener(&c);
    v.run();
}
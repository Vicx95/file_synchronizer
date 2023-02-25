#include "..//inc/FileSynchronizer.hpp"
#include "..//inc/Scanner.hpp"
#include "..//inc/controller.hpp"
#include "..//inc/model.hpp"
#include "..//inc/timer.hpp"
#include "..//inc/view.hpp"
#include "..//inc/Serializer.hpp"

#include <chrono> // chrono::system_clock
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    
    Timer tm;
    View v;
    Scanner scanner;
    FileSynchronizer sync;

    Model m(&tm, &sync, &scanner);
    Controller c(&v, &m);
    v.setListener(&c);
    v.run();
    
}
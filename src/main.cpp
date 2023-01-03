#include <iostream>
#include "..//inc/timer.hpp"

int main() {
    Timer tm;

    tm.start(std::chrono::milliseconds(1000), [] {
        std::puts("Synchronizuje!");
    });
    std::this_thread::sleep_for(std::chrono::seconds(10));
}
#include <iostream>
#include "..//inc/userInterface.hpp"

int main() {
    std::cout << "Hello from app!\n";
    UserInterface launchProgram;
    launchProgram.startSynchronizer();
    std::cout << "Bye app!\n";
    return 0;
}
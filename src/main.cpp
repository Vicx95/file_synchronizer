#include <iostream>
#include "..//inc/view.hpp"
#include "..//inc/model.hpp"
#include "..//inc/controller.hpp"
#include "..//inc/timer.hpp"

int main() {
    Timer tm;
    View v;
	Model m(&tm);
	Controller c(&v, &m);
	v.setListener(&c);
	v.run();
}
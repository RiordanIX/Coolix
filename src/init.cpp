#include <iostream>
#include "virt_comp.hpp"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "Usage:  coolix <run_file>" << std::endl;
		// Prvent warning using -Wall.
		std::cout << argc << " " << argv << std::endl;
	}
	else {
		virt_comp computer();
		computer.setup(argv);
		computer.run();
	}
}


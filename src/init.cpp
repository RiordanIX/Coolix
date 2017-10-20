#include <iostream>
#include "memory_constants.hpp"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "Usage:  coolix <run_file>" << std::endl;
	}
	else {
		OSDriver driver = OSDriver();
		driver.run(argv);
	}
}


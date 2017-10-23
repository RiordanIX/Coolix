#include <iostream>
#include "OSDriver.hpp"

int main(int argc, char* argv[]) {
#ifdef _WIN32
	std::cout << argc << '\n' << argv << std::endl;
	OSDriver driver;
	driver.run("test_job");
	return 0;
}
#endif // _WIN32
#ifndef _WIN32
#ifndef DEBUG
	if (argc == 1) {
		std::cout << "Usage:  coolix <run_file>" << std::endl;
	}
	else {
		OSDriver driver;
		driver.run(argv[1]);
	}
#else
	std::cout << argc << '\n' << argv << std::endl;
	OSDriver driver;
	driver.run("test_job");
#endif // DEBUG
	return 0;
}
#endif // _WIN32


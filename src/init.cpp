#include <iostream>
#include "OSDriver.hpp"

int main(int argc, char* argv[]) {
#if (defined _WIN32 || defined DEBUG)
	std::cout << argc << '\n' << argv << std::endl;
	OSDriver driver;
	driver.run("test_job2");
#else
	if (argc == 1) {
		std::cout << "Usage:  coolix <run_file>" << std::endl;
	}
	else {
		OSDriver driver;
		driver.run(argv[1]);
	}
#endif
	return 0;
}


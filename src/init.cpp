#include <iostream>
#include "OSDriver.hpp"

int main(int argc, char* argv[]) {
#if (defined _WIN32 && defined _DEBUG)
	std::cout << argc << '\n' << argv << std::endl;
	OSDriver driver;
	
	std::string jobfile;
	std::cout << "Please specify the job file you want to run: ";
	//std::cin >> jobfile;
	driver.run("test_job");
		
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


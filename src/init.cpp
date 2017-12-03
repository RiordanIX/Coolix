#include <iostream>
#include "OSDriver.hpp"
std::string sortby;
unsigned int cpucount;

int main(int argc, char* argv[]) {
#if (defined _WIN32 && defined _DEBUG)
	std::cout << "coolix os" << '\n' << argv << std::endl;
	std::cout << "('FIFO', 'P' , 'SJF') How you want to sort?: ";
	while (sortby.compare("FIFO") != 0
		&& sortby.compare("P") != 0
		&& sortby.compare("SJF") != 0)
	{
		std::cin >> sortby;
		if (sortby.compare("FIFO") != 0
			&& sortby.compare("P") != 0
			&& sortby.compare("SJF") != 0)
		{
			printf("please enter correct sort value \n 'FIFO', 'P' , 'SJF'");
		}
	}
	while (cpucount <= 0 || cpucount > 4)
	{
		std::cout << "How many cpu?: ";
		std::cin >> cpucount;
		if (cpucount <= 0 || cpucount > 4)
		{
			printf("please enter correct cpu count\n value must be between : 1 - 4 :");
		}
	}
	
	OSDriver driver;
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


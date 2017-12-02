#include <iostream>
#include "OSDriver.hpp"
std::string sortby;
unsigned int cpucount;

int main(int argc, char* argv[]) {
#if (defined _WIN32 && defined _DEBUG)
	std::cout << "coolix os" << '\n' << argv << std::endl;
	std::cout << "('FIFO', 'P' , 'SJF') How you want to sort?: ";
	while (!sortby._Equal("FIFO")
		&& !sortby._Equal("P")
		&& !sortby._Equal("SJF"))
	{
		std::cin >> sortby;
		if (!sortby._Equal("FIFO")
			&& !sortby._Equal("P")
			&& !sortby._Equal("SJF"))
		{
			printf("please enter correct sort value \n 'FIFO', 'P' , 'SJF'");
		}
	}
	while (cpucount == 0)
	{
		std::cout << "How many cpu?: ";
		std::cin >> cpucount;
		if (cpucount == 0)
		{
			printf("please enter correct cpu count");
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


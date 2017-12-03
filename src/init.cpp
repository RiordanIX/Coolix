#include <iostream>
#include "OSDriver.hpp"
std::string sortby;
unsigned int cpucount;
extern std::vector<PCB> process_list;

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
	std::ofstream analyiticFile;
	analyiticFile.open("AnalyticMicroSec.txt");
	analyiticFile << "PID\tIO\tMaxFrames\tMaxRam\tPercentCache\tPageFault\tFaultServiceTime\tWait\tRun" << std::endl;
	long int wtime;
	long int ttime;
	for (unsigned int i = 0; i < process_list.size(); i++)
	{
		wtime = process_list[i].get_wait_time_clock();	//	Wait Time
		ttime = process_list[i].get_exec_time_clock();	//	Total Time
		analyiticFile << process_list[i].get_pid()
			<< "\t" << process_list[i].get_io_count()
			<< "\t" << process_list[i].get_max_frames()
			<< "\t" << (process_list[i].get_max_frames() * PAGE_SIZE)
			<< "\t" << ((float)(process_list[i].get_cache_hit()) / (float)(process_list[i].get_cache_hit() + process_list[i].get_cache_miss()) * 100) << "\t" << process_list[i].get_page_fault_count()
			<< "\t" << process_list[i].get_page_fault_time_clock() << "\t" << wtime << "\t" << (ttime-wtime) << std::endl;
		  //<< "\t" << process_list[i].get_page_fault_service() << "\t" << (process_list[i].get_wait() / 4) << "\t" << process_list[i].get_run() << std::endl;
}

	analyiticFile.close();
	
		
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


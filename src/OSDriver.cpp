#include "OSDriver.hpp"

extern pcbQueue readyQueue;
extern FIFO waitingQueue, terminatedQueue , newQueue;
extern Ram MEM;

extern Disk DISK;
extern std::vector<PCB> process_list;

long int clock_Tick;
bool programEnd;
#if (defined DEBUG || defined _DEBUG)

#endif


OSDriver::OSDriver() :
		cpu_cycle(DEFAULT_CPU_CYCLE_TIME),
		current_cycle(0),
		ldr(),
		Dispatch(),
		ltSched(),
		CpuPool()
	{ }

OSDriver::~OSDriver() { }
void RunClock()
{
	while (programEnd == false)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));//increments 1 mirco sec
		clock_Tick++;
	}
}
void printOutPut(PCB * pcb)
{
	std::fstream myfile;
	myfile.open("example.txt", std::ios::out | std::ios::app);
	myfile << "Process completed :" << pcb->get_pid() << "\r\n";
	myfile.close();

}
void run_cpu(cpu * CPU, PCB * pcb, int * current_cycle)
{
	Hardware::LockHardware(pcb->get_resource_status()); //locks resource

	//set cpu cache id to running process
	CPU->cache.current_pid = pcb->get_pid();
	pcb->set_wait_end_clock();
	while (pcb->get_status() != status::TERMINATED &&
		pcb->get_status() != status::WAITING)
	{
		instruct_t instruct = CPU->fetch(pcb);
		if (pcb->get_status() == WAITING)
		{
			Dispatcher::switchOut(CPU, pcb);
			ShortTermScheduler::RunningToWait(pcb);
			break;
		}
		// The fetched instruction is 0, meaning it's accessed some zeroed out
		// data.  This shouldn't happen.
		if (instruct == 0) {
			auto note = pcb->get_frame(pcb->get_program_counter() / (PAGE_SIZE));
			std::cout << "Instruction at "
				<< note << " is 0\n"
				<< "Process page is "
				<< pcb->get_frame(pcb->get_program_counter() / (PAGE_SIZE))
				<< "\nProgram Counter is "
				<< pcb->get_program_counter()
				<< '\n';
			return;
		}
		if (pcb->get_status() == RUNNING)
		{	//  Decodes and Executes Instruction
			CPU->decode_and_execute(instruct, pcb);
		}
		if (pcb->get_status() == WAITING)
		{
			Dispatcher::switchOut(CPU, pcb);
			ShortTermScheduler::RunningToWait(pcb);
			break;
		}
		// Increment Program counter
		pcb->increment_PC();
		//increument cpu cycle
		CPU->current_cycle++;
		//osdriver cycle increument
		current_cycle++;
	}//while loop
	 //	Calculating Max Frames
	int vPages = 0;
	for (unsigned int i = 0; i < pcb->get_page_table_length(); i++)
	{
		if (pcb->is_valid_page(i))
			vPages++;
	}
	if (vPages > pcb->get_max_frames())
	{
		pcb->set_max_frames(vPages);
	}
	//	If process is terminated. Throw it into the Terminated Queue
	if (pcb->get_status() == TERMINATED)//if process not in waiting
	{	//  Since the Processes 'Should' be completed, it will be thrown into the TerminatedQueue
		pcb->set_end_clock();
		terminatedQueue.setLock();
		terminatedQueue.addProcess(pcb);
		printOutPut(pcb);
		terminatedQueue.freeLock();
		std::cout << "terminated :" << pcb->get_pid() << "\n";
		printf("dump pcb start\n");
		LongTerm::DumpProcess(pcb);
		printf("dump pcb end\n");
	}
	Hardware::FreeHardware(pcb->get_resource_status());//free resource for other processes
	CPU->freeLock();//free cpu lock when thread is completed

}
bool CheckRunLock(cpu * CPU)
{
	bool there = false;
	readyQueue.setLock();
	if (CPU != nullptr && readyQueue.size() > 0)
	{
		there = true;
	}
	readyQueue.freeLock();
	return there;
}
bool waitingQsize()
{
	bool there = false;
	waitingQueue.setLock();
	if (waitingQueue.size() > 0)
	{
		there = true;
	}
	waitingQueue.freeLock();
	return there;
}
bool TerminateQsize(int totalJobs)
{
	bool there = false;
	terminatedQueue.setLock();
	if (terminatedQueue.size()  < totalJobs )
	{
		there = true;
	}
	terminatedQueue.freeLock();
	return there;
}
void OSDriver::run(std::string fileName) {
	//  Load to Disk
	PCB * pcb;
	programEnd = false;
	std::thread RUN(RunClock);
	if (RUN.joinable() == true)
	{
		RUN.detach();
	}
	ldr.readFromFile(fileName);

	//  Does an initial load from Disk to RAM and ReadyQueue
	// loads only 4 pages per process
	debug_printf("Running Long term Scheduler%s", "\n");
	totalJobs = LongTerm::initialLoad();


#if (defined DEBUG || defined _DEBUG)
	debug_printf("Beginning MEMORY%s","\n");
	MEM.dump_data("Begin_RAM");
	DISK.dump_data("Begin_DISK");
#endif
	debug_printf("Finished with LongTerm Schdeduler%s","\n");
	//  Runs as long as the ReadyQueue is populated as long as there are
	//  processes to be ran
	cpu* CPU = CpuPool.FreeCPU();
	while(TerminateQsize(totalJobs))
	{
		try {
			if (CheckRunLock(CPU))
			{
				debug_printf("CPU is not null and ready queue is > 0%s","\n");
				if (CPU->getLock() == FREE)//check if cpu is still free
				{
					debug_printf("CPU is not Locked.%s","\n");
					run_shortts(CPU);//  Context Switches for the next process
					if (CPU->getLock() == LOCK)//make sure cpu is locked with a process first
					{
						std::thread RUN(run_cpu, CPU, CPU->getProcess(), &current_cycle);
						if (RUN.joinable() == true)
						{
							debug_printf("Creating new thread%s","\n");

							RUN.detach();
						}
					}
				}
			}
			else {
				debug_printf("CPU is null or ready queue is not > 0%s","\n");
			}
		}
		catch (const char* e) {
			// malfunctions print out
			printf("%s\n",e);
		}
		if (waitingQsize())//check if its greater than 0
		{
			debug_printf("Waiting Queue has things%s", "\n");
			waitingQueue.setLock();
			pcb = waitingQueue.getProcess();
			waitingQueue.freeLock();
			if (pcb->get_waitformmu() == true)
			{
				if (pcb->get_lastRequestedPage() < 256)
				{	//get requested frame
					if(ltSched.loadPage(pcb, pcb->get_lastRequestedPage()))
					{
						//if frame is aquired than set the wait for mmu event to false
						pcb->set_page_fault_end_clock();
						pcb->set_waitformmu(false);
					}
					else
					{
						if (ltSched.FrameSize() == 0)
						{
							//free up frames if there are no more frames left for
							//processes in ready queue
							waitingQueue.setLock();
							LongTerm::DumpFrame(pcb);
							debug_printf("Dumping Frame%s", "\n");
							waitingQueue.freeLock();
						}
					}
				}
			}
		}
		//  run short time scheduler to move processes
		//  from wait queue to ready or vice versa
		run_sortsch();
		CPU = CpuPool.FreeCPU(); //find free cpu to schedule next item in ready queue
		//debug_printf("This is doing stuff still%s", "\n");

	}
	programEnd = true; //stop cpu clock;

#if (defined DEBUG || defined _DEBUG)
	MEM.dump_data("End_RAM"); //memory dump
	DISK.dump_data("End_DISK");
#endif
	long int wtime;
	long int ttime;
	for (unsigned int i = 0; i < process_list.size(); i++)
	{
		wtime = process_list[i].get_wait_time_clock();	//	Wait Time
		ttime = process_list[i].get_exec_time_clock();	//	Total Time
		std::cout << "\n\nProcess id: " << process_list[i].get_pid()
			<< "\nIO count: " << process_list[i].get_io_count()
			<< "\nMaxFrames: " << process_list[i].get_max_frames()
			<< "\nMaxRam:" << (process_list[i].get_max_frames() * PAGE_SIZE) 
			<< "\nPercentCache:" << ((float)(process_list[i].get_cache_hit()) / (float)(process_list[i].get_cache_hit() + process_list[i].get_cache_miss()) * 100)
			<< "\ncpu0 count:" << process_list[i].get_cpu0_count() 
			<< "\ncpu1 count: " << process_list[i].get_cpu1_count() 
			<< "\ncpu2 count: " << process_list[i].get_cpu2_count() 
			<< "\ncpu3 count: " << process_list[i].get_cpu3_count()
			<< "\nPageFault count: " << process_list[i].get_page_fault_count()
			<< "\nFaultServiceTime: " << process_list[i].get_page_fault_time_clock() 
			<< "\nWait Time : " << wtime 
			<< "\nRun Time : " << (ttime - wtime) << std::endl;
	}
}

void OSDriver::run_sortsch() {
	// Populate RAM and ReadyQueue

	// Checks to see if any process in the Ready Queue should be moved to
	// Waiting Queue, then moves it
	StSched.ReadyToWait();

	// Checks to see if any process in the Waiting Queue should be moved to
	// Ready Queue, then moves it
	StSched.WaitToReady();
}
bool checkReadyQsize()
{
	bool there;
	readyQueue.setLock();
	if (!readyQueue.empty())
	{
		there = true;
	}
	else
	{
		there = false;
	}
	readyQueue.freeLock();
	return there;
}
void OSDriver::run_shortts(cpu * CPU) {
	// Dispatches the current Processes. Context Switches In AND Out
	if (checkReadyQsize())
	{
		Dispatch.dispatch(CPU, CPU->getProcess());
		if (current_cycle >= cpu_cycle)
			current_cycle = 0;
	}
}
void OSDriver::ClearCPU(unsigned int CpuID, unsigned int p_id)
{
	//clear cpu registers if same cpu doesn't take over process it was running
	//previously
	CPU_Pool::clearCpu(CpuID, p_id);
}


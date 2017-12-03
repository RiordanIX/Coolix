#include "OSDriver.hpp"

extern pcbQueue readyQueue;
extern FIFO waitingQueue, terminatedQueue , newQueue;

#if (defined DEBUG || defined _DEBUG)
extern Ram MEM;
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
	
	while (pcb->get_status() != status::TERMINATED &&
		pcb->get_status() != status::WAITING)
	{
		instruct_t instruct = CPU->fetch(pcb);
		if (pcb->get_status() == WAITING)
		{
			Dispatcher::switchOut(CPU,pcb);
			ShortTermScheduler::RunningToWait(pcb);
			break;
		}
		// The fetched instruction is 0, meaning it's accessed some zeroed out
		// data.  This shouldn't happen.
		if (instruct == 0) {
			auto note = pcb->get_frame(pcb->get_program_counter()/(PAGE_SIZE));
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
	//	If process is terminated. Throw it into the Terminated Queue
	if (pcb->get_status() == TERMINATED)//if process not in waiting
	{	//  Since the Processes 'Should' be completed, it will be thrown into the TerminatedQueue
		pcb->set_end_time();
		terminatedQueue.setLock();
		terminatedQueue.addProcess(pcb);
		//printOutPut(pcb);
		terminatedQueue.freeLock();
		std::cout << "terminated" << pcb->get_pid() << "\n";
		LongTerm::DumpProcess(pcb);
		//frame.unlock();
		//tq.unlock();
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
	ldr.readFromFile(fileName);

	//  Does an initial load from Disk to RAM and ReadyQueue
	// loads only 4 pages per process
	debug_printf("Running Long term Scheduler%s", "\n");
	totalJobs = LongTerm::initialLoad();

		
#if (defined DEBUG || defined _DEBUG)
	debug_printf("Beginning MEMORY%s","\n");
	MEM.dump_data();
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

				if (CPU->getLock() == FREE)
				{
					run_shortts(CPU);//  Context Switches for the next process
					std::thread RUN(run_cpu, CPU, CPU->getProcess(), &current_cycle);
					if (RUN.joinable() == true)
					{
						RUN.detach();
					}
				}
			}
		}
		catch (const char* e) {
			// malfunctions print out
			printf("%s\n",e);
		}
		if (waitingQsize())//check if its greater than 0
		{			
				waitingQueue.setLock();
				pcb = waitingQueue.getProcess();
				waitingQueue.freeLock();
				if (pcb->get_waitformmu() == true)
				{					
					if (pcb->get_lastRequestedPage() < 156)
					{	//get requested frame 
						if(ltSched.loadPage(pcb, pcb->get_lastRequestedPage()))
						{
							//if frame is aquired than set the wait for mmu event to false
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
		
	}


#if (defined DEBUG || defined _DEBUG)
	MEM.dump_data(); //memory dump
#endif
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
	




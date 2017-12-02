#include "OSDriver.hpp"

//#include "PriorityQueue.hpp"


extern PriorityQueue readyQueue;
extern FIFO waitingQueue, terminatedQueue , newQueue;

#if (defined DEBUG || defined _DEBUG)
extern Ram MEM;
#endif
int mmuLock = 0;
int writeInstruction = 0;
int waitQueueLock = 0;
int readyQueueLock = 0;
int terminatedQueueLock = 0;
int FrameDump = 0;



std::thread RUN;
using namespace std::chrono_literals;

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
	CPU->jobInAction = 1;
	Hardware::LockHardware(pcb->get_resource_status()); //locks resource
	//set pcb pointer to cpu local variable to keep track of running processes for each cpu
	//CPU->CurrentProcess = pcb;
	CPU->cache.current_pid = pcb->get_pid();
	//set process pcb to running status
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
			std::cout << "Decodes and Executes" << pcb->get_pid() << 
				 " Instruction : " << instruct << "\n";
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
		CPU->current_cycle++;
		current_cycle++;
		//std::this_thread::sleep_for(1ms);
	}
#if (defined DEBUG || defined _DEBUG)
	//PCB* p = readyQueue.getProcess();
	/*
	printf("Ram Address:\t%lu\n", CPU->CurrentProcess->get_ram_address());
	for (unsigned int i = CPU->CurrentProcess->get_ram_address() + CPU->CurrentProcess->get_out_address();
	i < CPU->CurrentProcess->get_ram_address() + CPU->CurrentProcess->get_end_address();
	i+=4) {
	printf("Output %d:\t%d\n", i, MEM.get_instruction(i));
	}*/
#endif // DEBUG
	if (pcb->get_status() == TERMINATED)//if process not in waiting
	{	//  Since the Processes 'Should' be completed, it will be thrown into the TerminatedQueue
		pcb->set_end_time();
		while (terminatedQueueLock == 1) { printf("terminated"); }
		terminatedQueueLock = 1;
		terminatedQueue.addProcess(pcb);
		//printOutPut(pcb);
		terminatedQueueLock = 0;
		LongTerm::DumpProcess(pcb);
		//frame.unlock();
		//tq.unlock();
	}
	Hardware::FreeHardware(pcb->get_resource_status());//free resource for other processes
	CPU->jobInAction = 0;																   //readyQueue.removeProcess();
	/*if (RUN.joinable() == true)
	{
		RUN.join();
	}
	else
	{
		try {
			RUN.~thread();
		}
		catch (std::exception &ee) {}
	}*/
	CPU->freeLock();
}
bool CheckRunLock(cpu * CPU)
{
	bool there = false;
	while (readyQueueLock == 1) { printf("readyQ"); }
	readyQueueLock = 1;
	if (CPU != nullptr && readyQueue.size() > 0)
	{
		there = true;
	}
	readyQueueLock = 0;
	return there;
}
bool waitingQsize()
{
	bool there = false;
	while (waitQueueLock == 1) { printf("waitingQ"); }
	waitQueueLock = 1;
	if (waitingQueue.size() > 0)
	{
		there = true;
	}
	waitQueueLock = 0;
	return there;
}
bool TerminateQsize(int totalJobs)
{
	bool there = false;
	while (terminatedQueueLock == 1) { printf("terminated"); }
	terminatedQueueLock = 1;
	if (terminatedQueue.size()  < totalJobs )
	{
		there = true;
	}
	terminatedQueueLock = 0;
	return there;
}
void OSDriver::run(std::string fileName) {
	//  Load to Disk
	PCB * pcb;
	ldr.readFromFile(fileName);


	//  Does an initial load from Disk to RAM and ReadyQueue
	debug_printf("Running Long term Scheduler%s","\n");
	while (!newQueue.empty())
	{
	//	if (newQueue.getProcess()->get_pid() == 28)
		{
			ltSched.loadProcess(newQueue.getProcess(), 0);
			totalJobs++;
		}
		newQueue.removeProcess();
	}
	
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
				//remove process from the ready queue
				//while (rq.try_lock() == false) {}
				if (CPU->jobInAction == 0)
				{
				   //rq.unlock();
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
			
		}
		catch (const char* e) {
			//  Remove the process if it malfunctions
			printf("%s\n",e);
			//readyQueue.removeProcess(); we pop the queue when we ran the process already
		}
		//std::this_thread::sleep_for(2ms);
		if (waitingQsize())//check if its greater than 0
		{
			
				while (waitQueueLock == 1)
				{ printf("waitingQ"); }
				waitQueueLock = 1;
				pcb = waitingQueue.getProcess();
				waitQueueLock = 0;
				if (pcb->get_waitformmu() == true)
				{					
					if (pcb->get_lastRequestedPage() < 156)
					{
						if(ltSched.loadPage(pcb, pcb->get_lastRequestedPage()))
						{
							pcb->set_waitformmu(false);
						}
						else
						{
							if (ltSched.FrameSize() == 0)
							{
								while (waitQueueLock == 1) { printf("waitingQ"); }
								waitQueueLock = 1;
								if (pcb->get_registers()[5] == 0 && pcb->get_registers()[9] > 0)
								{
									printf("register 5 = 0\n");
								}
								LongTerm::DumpFrame(pcb);
								waitQueueLock = 0;
							}
						}
					}
					//StSched.WaitToReady();
				}
		}
		//  Load and Move Processes accordingly
		run_longts();
		CPU = CpuPool.FreeCPU();
		
	}


#if (defined DEBUG || defined _DEBUG)
	MEM.dump_data();
#endif

	//  Calcualtes the AverageCycleRunTime
	/*
	int averageCycleRunTime = 0;
	for(int i = 0; i < terminatedQueue.size(); i++)
	{
		averageCycleRunTime += (terminatedQueue.getProcess()->get_cycle_start_time());
		terminatedQueue.removeProcess();
	}
	*/
}

void OSDriver::print_error(PCB* p) {
	auto note = p->get_ram_address() + p->get_program_counter();
	std::cout << "Instruction at "
			<< note << " is 0\n"
			<< "Process Ram address is "
			<< p->get_ram_address()
			<< "\nProgram Counter is "
			<< p->get_program_counter()
			<< '\n';
}

void OSDriver::run_longts() {
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
	while (readyQueueLock == 1) { printf("readyQ"); }
	readyQueueLock = 1;
	if (!readyQueue.empty())
	{
		there = true;
	}
	else
	{
		there = false;
	}
	readyQueueLock = 0;
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
	CPU_Pool::clearCpu(CpuID, p_id);
}
	




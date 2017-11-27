#include "OSDriver.hpp"
//#include "PriorityQueue.hpp"

extern PriorityQueue terminatedQueue;
extern PriorityQueue readyQueue, newQueue , waitingQueue;

#if (defined DEBUG || defined _DEBUG)
extern Ram MEM;
#endif
std::mutex tq,ex, frame;
std::mutex wq;
std::mutex rq;
std::thread RUN;

OSDriver::OSDriver() :
		cpu_cycle(DEFAULT_CPU_CYCLE_TIME),
		current_cycle(0),
		totalJobs(0),
		ldr(),
		Dispatch(),
		ltSched()
	{ }

OSDriver::~OSDriver() { }

void run_cpu(cpu * CPU, PCB * pcb, int * current_cycle)
{
	//Hardware::LockHardware(pcb->get_resource_status()); //locks resource
	//set pcb pointer to cpu local variable to keep track of running processes for each cpu
	CPU->CurrentProcess = pcb;
	CPU->cache.current_pid = pcb->get_pid();
	//set process pcb to running status
	while (CPU->CurrentProcess->get_status() != status::TERMINATED &&
		CPU->CurrentProcess->get_status() != status::WAITING)
	{
		debug_printf("Status:\t%d\n", CPU->CurrentProcess->get_status());
		instruct_t instruct = CPU->fetch(CPU->CurrentProcess);
		if (CPU->CurrentProcess->get_status() == WAITING)
		{
			ShortTermScheduler::RunningToWait(CPU->CurrentProcess);
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
		if (CPU->CurrentProcess->get_status() == RUNNING)
		{	//  Decodes and Executes Instruction
			CPU->decode_and_execute(instruct, CPU->CurrentProcess);
		}
		if (CPU->CurrentProcess->get_status() == WAITING)
		{
			ShortTermScheduler::RunningToWait(CPU->CurrentProcess);
			break;
		}
		// Increment Program counter
		CPU->CurrentProcess->increment_PC();
		CPU->current_cycle++;
		current_cycle++;
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
	if (CPU->CurrentProcess->get_status() == TERMINATED)//if process not in waiting
	{	//  Since the Processes 'Should' be completed, it will be thrown into the TerminatedQueue
		while (tq.try_lock() == false) {
			debug_printf("Waiting on Terminated Queue%s","\n");
			}
		terminatedQueue.addProcess(CPU->CurrentProcess);
		while (frame.try_lock()) {
			debug_printf("Waiting on Frame %s","\n");
		}
		LongTerm::DumpProcess(CPU->CurrentProcess);
		frame.unlock();
		tq.unlock();
		debug_printf("Everything is unlocked%s","\n");

	}
	//Hardware::FreeHardware(CPU->CurrentProcess->get_resource_status());//free resource for other processes
																	   //readyQueue.removeProcess();
	//while ( true) {
		if (RUN.joinable() == true) {
			debug_printf("Rejoining thread%s","\n");
			RUN.join();
			//break;
		}
		else {
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
			printf("Waiting to rejoin");
			try {
				RUN.~thread();
			}
			catch (std::exception &ee) {
				printf("%s: due to threading issues\n", ee.what());
			}
		}
		debug_printf("End of run_cpu%s","\n");

	//}
}


void OSDriver::run(std::string fileName) {
	//  Load to Disk
	PCB * pcb;
	ldr.readFromFile(fileName);


	//  Does an initial load from Disk to RAM and ReadyQueue
	debug_printf("Running Long term Scheduler%s","\n");
	while (!newQueue.empty())
	{
		ltSched.loadProcess(newQueue.getProcess(),0);
		newQueue.removeProcess(); totalJobs++;
	}
#if (defined DEBUG || defined _DEBUG)
	debug_printf("Beginning MEMORY%s","\n");
	MEM.dump_data();
#endif
	debug_printf("Finished with LongTerm Schdeduler%s","\n");
	//  Runs as long as the ReadyQueue is populated as long as there are
	//  processes to be ran
	cpu* CPU = CPU_Pool::FreeCPU();
	while(terminatedQueue.size() < totalJobs)
	{

		debug_printf("Total Jobs::\t%d\n", totalJobs);
		debug_printf("Terminated Queue:\t%d\n", terminatedQueue.size());
		try {

			if (CPU != nullptr && readyQueue.size() > 0)
			{
				//remove process from the ready queue
				while (rq.try_lock() == false) {}
				CPU->CurrentProcess = readyQueue.getProcess();
				CPU->CurrentProcess->set_status(RUNNING);
				readyQueue.removeProcess();
				rq.unlock();
				std::thread RUN(run_cpu, CPU, CPU->CurrentProcess, &current_cycle);
				if (RUN.joinable() == true)
				{
					RUN.detach();
				}
			}
		}
		catch (const char* e) {
			//  Remove the process if it malfunctions
			printf("%s in the OSDriver\n",e);
			//return;
			//readyQueue.removeProcess(); we pop the queue when we ran the process already
		}
		if (waitingQueue.size() > 0)
		{

				while (wq.try_lock() == false) {}
				pcb = waitingQueue.getProcess();
				wq.unlock();
				if (pcb->get_waitformmu() == true)
				{

					if (pcb->get_lastRequestedPage() < 256)
					{
						if(ltSched.loadPage(pcb, pcb->get_lastRequestedPage()))
						{
							pcb->set_waitformmu(false);
						}
					}
					StSched.WaitToReady();
				}
		}
		//  Load and Move Processes accordingly
		run_longts();
		CPU = CPU_Pool::FreeCPU();
		//  Context Switches for the next process
		if(CPU != nullptr && CPU->CurrentProcess != nullptr)
		if ((readyQueue.size() > 0 || (waitingQueue.size() > 0)))
		{
			try
			{
				if (CPU->CurrentProcess->get_status() != RUNNING)
				{
					run_shortts(CPU_Pool::FreeCPU());
				}
			}
			catch (std::exception) {}
		}
		if (waitingQueue.size() > 0 && readyQueue.size() == 0)
		{

		}
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

void OSDriver::run_shortts(cpu * CPU) {
	// Dispatches the current Processes. Context Switches In AND Out
	if (!readyQueue.empty())
	{
		Dispatch.dispatch(CPU, CPU->CurrentProcess);
		if(current_cycle >= cpu_cycle)
			current_cycle = 0;
	}

}


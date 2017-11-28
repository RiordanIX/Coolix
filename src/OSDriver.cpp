#include "OSDriver.hpp"

extern PriorityQueue terminatedQueue;
extern PriorityQueue readyQueue;

#if (defined DEBUG || defined _DEBUG)
extern Ram MEM;
#endif


//std::mutex mtx;
std::thread RUN;


OSDriver::OSDriver() :
		cpu_cycle(DEFAULT_CPU_CYCLE_TIME),
		current_cycle(0),
		ldr(),
		Dispatch(),
		CPU(),
		ltSched() {}



void run_cpu(cpu *CPU, PCB *Pcb, int *current_cycle) {
	CPU->currentProcess = Pcb;
	//readyQueue.removeProcess();
	Pcb->set_status(RUNNING);
	while (Pcb->get_status() != status::TERMINATED) {
		instruct_t instruct = fetch(CPU, Pcb);

		// The fetched instruction is 0, meaning it's accessed some zeroed out
		// data.  This shouldn't happen.
		if (instruct == 0) {

			auto p = Pcb;
			auto note = p->get_ram_address() + p->get_program_counter();
			std::cout << "Instruction at "
				<< note << " is 0\n"
				<< "Process Ram address is "
				<< p->get_ram_address()
				<< "\nProgram Counter is "
				<< p->get_program_counter()
				<< '\n';
		}
		//  Decodes and Executes Instruction
		CPU->decode_and_execute(instruct, Pcb);

		// Increment Program counter
		Pcb->increment_PC();
		CPU->current_cycle++;
		current_cycle++;
	}
#if (defined DEBUG || defined _DEBUG)
	PCB* p = Pcb;
	printf("Ram Address:\t%lu\n", p->get_ram_address());
	for (unsigned int i = p->get_ram_address() + p->get_out_address();
		i < p->get_ram_address() + p->get_end_address();
		i += 4) {
		printf("Output %d:\t%d\n", i, MEM.get_instruction(i));
	}
#endif // DEBUG

	//  Since the Processes 'Should' be completed, it will be thrown into the TerminatedQueue
	terminatedQueue.addProcess(Pcb);
	if (RUN.joinable() == true) {
		RUN.join();
	}
	else {
		try {
			RUN.~thread();
		}
		catch (std::exception &e) {
			printf("While Joining:\t%s\n",e.what());
		}
	}

	//readyQueue.removeProcess();

}


void OSDriver::run(std::string fileName)
{
	//  Load to Disk
	ldr.readFromFile(fileName);
	//  Does an initial load from Disk to RAM and ReadyQueue
	debug_printf("Running Long term Scheduler%s","\n");
	run_longts();
	//  Runs as long as the ReadyQueue is populated / as long as there are processes to be ran
	cpu * CPU = CPU_Pool::FreeCPU();// readyQueue.removeProcess();

	while(readyQueue.size() > 0)
	{
		//  Load and Move Processes accordingly
		run_longts();
		try {
			//  Runs the CPU
			CPU = CPU_Pool::FreeCPU();
			if (CPU != nullptr )
			{
				std::thread RUN(run_cpu, CPU, readyQueue.getProcess(), &current_cycle);
				readyQueue.removeProcess();
				//if (RUN.joinable() == true)
				{
					RUN.detach();
				}
			}
		}
		catch (const char* e) {
			//  Remove the process if it malfunctions
			printf("%s\n",e);
			//readyQueue.removeProcess();
		}
	//	Context Switches for the next process
	if (CPU != nullptr)
		{
		//	run_shortts(CPU);
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


/*
void OSDriver::run_cpu()
{
	while(readyQueue.getProcess()->get_status() != status::TERMINATED)
	{
		instruct_t instruct = CPU.fetch(readyQueue.getProcess());

		// The fetched instruction is 0, meaning it's accessed some zeroed out
		// data.  This shouldn't happen.
		if (instruct == 0) {

			auto p = readyQueue.getProcess();
			auto note = p->get_ram_address() + p->get_program_counter();
			std::cout << "Instruction at "
					<< note << " is 0\n"
					<< "Process Ram address is "
					<< p->get_ram_address()
					<< "\nProgram Counter is "
					<< p->get_program_counter()
					<< '\n';
		}
		//  Decodes and Executes Instruction
		CPU.decode_and_execute(instruct, readyQueue.getProcess());

		// Increment Program counter
		readyQueue.getProcess()->increment_PC();
		current_cycle++;
	}
#if (defined DEBUG || defined _DEBUG)
	PCB* p = readyQueue.getProcess();
	printf("Ram Address:\t%lu\n", p->get_ram_address());
	for (unsigned int i = p->get_ram_address() + p->get_out_address();
			i < p->get_ram_address() + p->get_end_address();
			i+=4) {
		printf("Output %d:\t%d\n", i, MEM.get_instruction(i));
	}
#endif // DEBUG

	//  Since the Processes 'Should' be completed, it will be thrown into the TerminatedQueue
	terminatedQueue.addProcess(readyQueue.getProcess());
	readyQueue.removeProcess();

}
*/


void OSDriver::run_longts()
{
	//  Populate RAM and ReadyQueue
	ltSched.DiskToRam();

	//  Checks to see if any process in the Ready Queue should be moved to
	//  Waiting Queue, then moves it
	//ltSched.ReadyToWait();

	//  Checks to see if any process in the Waiting Queue should be moved to
	//  Ready Queue, then moves it
	//ltSched.WaitToReady();
}

void OSDriver::run_shortts(cpu* CPU)
{
	// Dispatches the current Processes. Context Switches In AND Out
	if (!readyQueue.empty()) {
		Dispatch.dispatch(CPU, readyQueue.getProcess());
		if (current_cycle >= cpu_cycle) {
			current_cycle = 0;
		}
	}
}


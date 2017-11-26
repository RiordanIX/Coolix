#include "OSDriver.hpp"
//#include "PriorityQueue.hpp"

extern PriorityQueue terminatedQueue;
extern PriorityQueue readyQueue, newQueue;

#if (defined DEBUG || defined _DEBUG)
extern Ram MEM;
#endif

OSDriver::OSDriver() :
		cpu_cycle(DEFAULT_CPU_CYCLE_TIME),
		current_cycle(0),
		ldr(),
		Dispatch(),
		ltSched()
	{ }

OSDriver::~OSDriver() { }

void OSDriver::run(std::string fileName) {
	//  Load to Disk
	ldr.readFromFile(fileName);


	//  Does an initial load from Disk to RAM and ReadyQueue
	debug_printf("Running Long term Scheduler%s","\n");
	while (!newQueue.empty())
	{
		ltSched.loadProcess();
	}
#if (defined DEBUG || defined _DEBUG)
	debug_printf("Beginning MEMORY%s","\n");
	MEM.dump_data();
#endif
	debug_printf("Finished with LongTerm Schdeduler%s","\n");
	//  Runs as long as the ReadyQueue is populated as long as there are
	//  processes to be ran
	cpu* CPU;
	while(readyQueue.size() > 0)
	{
		//  Load and Move Processes accordingly
		run_longts();
		try {

			CPU = CPU_Pool::FreeCPU();
			if (CPU != nullptr)
			{
				//  Runs the CPU
				run_cpu(CPU);
			}
		}
		catch (const char* e) {
			//  Remove the process if it malfunctions
			printf("%s\n",e);
			//readyQueue.removeProcess(); we pop the queue when we ran the process already
		}
		//  Context Switches for the next process
	//	run_shortts(CPU_Pool::FreeCPU());
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

void OSDriver::run_cpu(cpu * CPU) {

	Hardware::LockHardware(readyQueue.getProcess()->get_resource_status()); //locks resource
	//set pcb pointer to cpu local variable to keep track of running processes for each cpu
	CPU->CurrentProcess = readyQueue.getProcess();
	readyQueue.removeProcess(); //remove process from the ready queue
	CPU->CurrentProcess->set_status(RUNNING);//set process pcb to running status
	while(CPU->CurrentProcess->get_status() != status::TERMINATED ||
			CPU->CurrentProcess->get_status() != status::WAITING)
	{
		instruct_t instruct = CPU->fetch(CPU->CurrentProcess);

		// The fetched instruction is 0, meaning it's accessed some zeroed out
		// data.  This shouldn't happen.
		if (instruct == 0) {
			print_error(CPU->CurrentProcess);
			return;
		}
		//  Decodes and Executes Instruction
		CPU->decode_and_execute(instruct, CPU->CurrentProcess);

		// Increment Program counter
		CPU->CurrentProcess->increment_PC();
		CPU->current_cycle++;
		current_cycle++;
	}
#if (defined DEBUG || defined _DEBUG)
	//PCB* p = readyQueue.getProcess();
	printf("Ram Address:\t%lu\n", CPU->CurrentProcess->get_ram_address());
	for (unsigned int i = CPU->CurrentProcess->get_ram_address() + CPU->CurrentProcess->get_out_address();
			i < CPU->CurrentProcess->get_ram_address() + CPU->CurrentProcess->get_end_address();
			i+=4) {
		printf("Output %d:\t%d\n", i, MEM.get_instruction(i));
	}
#endif // DEBUG

	//  Since the Processes 'Should' be completed, it will be thrown into the TerminatedQueue
	terminatedQueue.addProcess(CPU->CurrentProcess);
	Hardware::FreeHardware(CPU->CurrentProcess->get_resource_status());//free resource for other processes
	//readyQueue.removeProcess();

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

void OSDriver::run_shortts(cpu CPU) {
	// Dispatches the current Processes. Context Switches In AND Out
	if (!readyQueue.empty()) {
		Dispatch.dispatch(&CPU, readyQueue.getProcess());
		if(current_cycle >= cpu_cycle)
			current_cycle = 0;
	}
}


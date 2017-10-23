#include "OSDriver.hpp"
#include "PriorityQueue.h"

extern PriorityQueue terminatedQueue;

#ifdef DEBUG
extern Ram MEM;
#endif

OSDriver::OSDriver() :
		cpu_cycle(DEFAULT_CPU_CYCLE_TIME),
		current_cycle(0),
		ldr(),
		Dispatch(),
		CPU(),
		ltSched()
	{ }

OSDriver::~OSDriver()
{

}

void OSDriver::run(std::string fileName)
{
    //  Load to Disk
	ldr.readFromFile(fileName);
    //  Does an initial load from Disk to RAM and ReadyQueue
#ifdef DEBUG
	printf("Running Long term Scheduler\n");
#endif
	run_longts();
    //  Runs as long as the ReadyQueue is populated / as long as there are processes to be ran
	while(readyQueue.size() > 0)
	{
        //  Load and Move Processes accordingly
		run_longts();
		try {
            //  Runs the CPU
			run_cpu();
		}
		catch (const char* e) {
            //  Remove the process if it malfunctions
			printf("%s\n",e);
			readyQueue.removeProcess();
		}
        //  Context Switches for the next process
		run_shortts();
	}

#ifdef DEBUG
	for (unsigned int i = 0; i < MEM.size(); i +=6*4) {
		for (unsigned int j = i; j < i + 6*4 && j < MEM.size(); j+=4) {
			std::cout << j << ":\t" << MEM.get_instruction(j) << "\t";
		}
		printf("\n");
	}
	// To flush the stream
	std::cout << std::endl;
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


void OSDriver::run_cpu()
{
	while(readyQueue.getProcess()->get_status() != status::TERMINATED)
	{
        //  Fetches instruction
		instruct_t instruct = CPU.fetch(readyQueue.getProcess());
#ifdef DEBUG
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
#endif
        //  Decodes and Executes Instruction
		CPU.decode_and_execute(instruct, readyQueue.getProcess());

		// Increment Program counter
		readyQueue.getProcess()->increment_PC();
		current_cycle++;
	}
#ifdef DEBUG
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


void OSDriver::run_longts()
{
	ltSched.DiskToRam();	//  Populate RAM and ReadyQueue
	ltSched.ReadyToWait();	//  Checks to see if any process in the Ready Queue should be moved to Waiting Queue, then moves it
	ltSched.WaitToReady();	//  Checks to see if any process in the Waiting Queue should be moved to Ready Queue, then moves it
}

void OSDriver::run_shortts()
{
    // Dispatches the current Processes. Context Switches In AND Out
	if (!readyQueue.empty()) {
		Dispatch.dispatch(&CPU, readyQueue.getProcess());
		if(current_cycle >= cpu_cycle)
			current_cycle = 0;
	}
}


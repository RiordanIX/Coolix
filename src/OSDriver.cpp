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
    ldr.readFromFile(fileName);  //  Load to Disk
    run_longts();
    while(readyQueue.size() > 0)
    {
        run_longts();
        run_cpu();
        run_shortts();
    }

    int averageCycleRunTime;
    for(int i = 0; i < terminatedQueue.size(); i++)
    {
        averageCycleRunTime += (terminatedQueue.getProcess()->get_cycle_start_time());
        terminatedQueue.removeProcess();
    }
}


void OSDriver::run_cpu()
{
    while(readyQueue.getProcess()->get_status() != status::TERMINATED)
    {
		instruct_t instruct = CPU.fetch(readyQueue.getProcess());
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

	terminatedQueue.addProcess(readyQueue.getProcess());
	readyQueue.removeProcess();

}


void OSDriver::run_longts()
{
    ltSched.DiskToRam();                //  Populate RAM and ReadyQueue
    ltSched.ReadyToWait();              //  Checks to see if any process in the Ready Queue should be moved to Waiting Queue, then moves it
    ltSched.WaitToReady();              //  Checks to see if any process in the Waiting Queue should be moved to Ready Queue, then moves it
}

void OSDriver::run_shortts()
{
	// Only
    Dispatch.dispatch(&CPU, readyQueue.getProcess(), current_cycle, cpu_cycle);
    if(current_cycle >= cpu_cycle)
        current_cycle = 0;
}


#include "OSDriver.hpp"

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
}


void OSDriver::run_cpu()
{
	instruct_t instruct = CPU.fetch(readyQueue.getProcess());
    CPU.decode_and_execute(instruct, readyQueue.getProcess());
	// Increment Program counter
	readyQueue.getProcess()->increment_PC();
    current_cycle++;
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


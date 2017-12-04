#include "dispatcher.hpp"
#include "pcbQueue.hpp"
#include "OSDriver.hpp"

extern FIFO terminatedQueue;
extern FIFO waitingQueue;
extern pcbQueue readyQueue;
//extern int total_cycles;
bool checkReadysize()
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
void Dispatcher::dispatch(cpu* CPU) {
	debug_printf("I am trying to dispatch%s","\n");
	if (checkReadysize())
	{
		debug_printf("I am dipatching !!!!!!!!!!!!!!!!!!!!!!!!!%s","\n");

		// Removes First Process and gives next process to CPU

		switchIn(CPU);

	}
}


// Assign's current CPU registers to PCB registers. Context Switches.
void Dispatcher::switchOut(cpu* CPU, PCB* cProcess) {
	if (CPU->getProcess() != nullptr)
	{
		if (CPU->getId() == cProcess->get_cpuid())
		{
			// Copy current process registers to pcb
			cProcess->set_registers(CPU->registers);
		}
	}
}


void Dispatcher::switchIn(cpu* CPU)
{
	debug_printf("Trying to switch In%s","\n");

	if (checkReadysize())
	{
		CPU->setLock();
		readyQueue.setLock();
		// Sets the CPU registers to the new PCB registers
		CPU->set_registers(readyQueue.getProcess()->get_registers());
		CPU->setProcess(readyQueue.getProcess());

		//clear any cpu's registers if they were running the same process
		OSDriver::ClearCPU(CPU->getId(), CPU->getProcess()->get_pid());
		readyQueue.removeProcess();
		readyQueue.freeLock();
		CPU->getProcess()->set_cpuid(CPU->getId());
		CPU->getProcess()->set_status(status::RUNNING);
		debug_printf("Correctly swapped processes!!!!%s","\n");
	}
	else {
		debug_printf("Failed to switch In%s","\n");
	}

}


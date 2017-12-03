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
void Dispatcher::dispatch(cpu* CPU, PCB* cProcess) {
	debug_printf("I am trying to dispatch%s","\n");
	if (checkReadysize())
	{
		debug_printf("I am dipatching !!!!!!!!!!!!!!!!!!!!!!!!!%s","\n");
	
		// Removes First Process and gives next process to CPU

		switchIn(CPU,cProcess);
		
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


void Dispatcher::switchIn(cpu* CPU,PCB * cProcess) 
{
	
	readyQueue.setLock();
	if (!readyQueue.empty())
	{
		CPU->setLock();
		CPU->setProcess(readyQueue.getProcess());
		readyQueue.removeProcess();
		//clear any cpu's registers if they were running the same process
		OSDriver::ClearCPU(CPU->getId(),CPU->getProcess()->get_pid());
		// Sets the CPU registers to the new PCB registers
		CPU->set_registers(CPU->getProcess()->get_registers());
		CPU->getProcess()->set_cpuid(CPU->getId());
		CPU->getProcess()->set_status(status::RUNNING);
		
		debug_printf("Correctly swapped processes!!!!%s","\n");
	}
	readyQueue.freeLock();
}


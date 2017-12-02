#include "dispatcher.hpp"
#include "PriorityQueue.hpp"
#include "OSDriver.hpp"

extern FIFO terminatedQueue;
extern FIFO waitingQueue;
extern PriorityQueue readyQueue;
//extern int total_cycles;
extern int readyQueueLock;
bool checkReadysize()
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
			cProcess->set_registers(CPU->registers);
		}
	}
}


void Dispatcher::switchIn(cpu* CPU,PCB * cProcess) {
	//	If process is terminated. Throw it into the Terminated Queue
	if (CPU->getProcess() != nullptr)
	{
		if (CPU->getProcess()->get_status() == status::TERMINATED)
		{
			//PCB* exitingProcess = CPU->CurrentProcess;
			//terminatedQueue.addProcess(exitingProcess);
			
		}
	}

	// Removes the Current Running Process, so the next one will be the new
	// active process
	//readyQueue.removeProcess();
	// Sets the CPU registers to the new PCB registers
	while (readyQueueLock == 1) { printf("readyQ"); }
	readyQueueLock = 1;
	// Copy current process registers to pcb
	if (!readyQueue.empty())
	{
		CPU->setLock();
		CPU->setProcess(readyQueue.getProcess());
		readyQueue.removeProcess();
		OSDriver::ClearCPU(CPU->getId(),CPU->getProcess()->get_pid());
		CPU->set_registers(CPU->getProcess()->get_registers());
		CPU->getProcess()->set_cpuid(CPU->getId());
		CPU->getProcess()->set_status(status::RUNNING);
		
		debug_printf("Correctly swapped processes!!!!%s","\n");
	}
	readyQueueLock = 0;
}


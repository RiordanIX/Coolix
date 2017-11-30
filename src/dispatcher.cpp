#include "dispatcher.hpp"
#include "PriorityQueue.hpp"
#include "OSDriver.hpp"

extern FIFO terminatedQueue;
extern FIFO waitingQueue;
extern PriorityQueue readyQueue;
//extern int total_cycles;
extern int readyQueueLock;
void Dispatcher::dispatch(cpu* CPU, PCB* cProcess) {
	debug_printf("I am trying to dispatch%s","\n");
	if (!readyQueue.empty())
	{
		debug_printf("I am dipatching !!!!!!!!!!!!!!!!!!!!!!!!!%s","\n");
	
		// Removes First Process and gives next process to CPU

		switchIn(CPU,cProcess);
		
	}
}


// Assign's current CPU registers to PCB registers. Context Switches.
void Dispatcher::switchOut(cpu* CPU, PCB* cProcess) {
	if(CPU->CurrentProcess != nullptr)
	cProcess->set_registers(CPU->registers);
}


void Dispatcher::switchIn(cpu* CPU,PCB * cProcess) {
	//	If process is terminated. Throw it into the Terminated Queue
	if (CPU->CurrentProcess != nullptr)
	{
		if (CPU->CurrentProcess->get_status() == status::TERMINATED)
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
	// Moves current process to WaitingQueue
	switchOut(CPU, cProcess);
	if (!readyQueue.empty())
	{
		CPU->set_registers(readyQueue.getProcess()->get_registers());
		CPU->CurrentProcess = readyQueue.getProcess();
		readyQueue.getProcess()->set_status(status::RUNNING);
		readyQueue.removeProcess();
		debug_printf("Correctly swapped processes!!!!%s","\n");
	}
	readyQueueLock = 0;
}


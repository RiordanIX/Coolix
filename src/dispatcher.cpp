#include "dispatcher.hpp"
#include "PriorityQueue.hpp"
#include "OSDriver.hpp"

extern PriorityQueue terminatedQueue;
extern PriorityQueue readyQueue;
//extern int total_cycles;

void Dispatcher::dispatch(cpu* CPU, PCB* cProcess) {
	debug_printf("I am trying to dispatch%s","\n");
	if (!readyQueue.empty())
	{
		debug_printf("I am dipatching !!!!!!!!!!!!!!!!!!!!!!!!!%s","\n");
		// Moves current process to WaitingQueue
		switchOut(CPU, cProcess);
		// Removes First Process and gives next process to CPU
		switchIn(CPU);
	}
}


// Assign's current CPU registers to PCB registers. Context Switches.
void Dispatcher::switchOut(cpu* CPU, PCB* cProcess) {
	cProcess->set_registers(CPU->registers);
}


void Dispatcher::switchIn(cpu* CPU) {
	//	If process is terminated. Throw it into the Terminated Queue
	if(readyQueue.getProcess()->get_status() == status::TERMINATED)
	{
		PCB* exitingProcess = readyQueue.getProcess();
		//exitingProcess->set_end_time();
		terminatedQueue.addProcess(exitingProcess);
	}

	// Removes the Current Running Process, so the next one will be the new
	// active process
	//readyQueue.removeProcess();
	// Sets the CPU registers to the new PCB registers
	if (!readyQueue.empty())
	{
		CPU->set_registers(readyQueue.getProcess()->get_registers());
		readyQueue.getProcess()->set_status(status::RUNNING);
		readyQueue.removeProcess();
		debug_printf("Correctly swapped processes!!!!%s","\n");
	}
}


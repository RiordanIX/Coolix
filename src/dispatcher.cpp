#include "dispatcher.hpp"
#include "PriorityQueue.h"
#include "OSDriver.hpp"

extern PriorityQueue terminatedQueue;
//extern int total_cycles;

void Dispatcher::dispatch(cpu* CPU, PCB* cProcess) {
	// If the readyQueue is not empty
#ifdef DEBUG
	printf("I am trying to dispatch\n");
#endif
	if (!readyQueue.empty())
	{
#ifdef DEBUG
		printf("I am dipatching !!!!!!!!!!!!!!!!!!!!!!!!!\n");
#endif
		switchOut(CPU, cProcess);   //  Moves current process to WaitingQueue
		switchIn(CPU);			  //  Removes First Process and gives next process to CPU
	}
}


void Dispatcher::switchOut(cpu* CPU, PCB* cProcess) {
    //  Assign's current CPU registers to PCB registers. Context Switches.
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

	// Removes the Current Running Process, so the next one will be the new active process
	//readyQueue.removeProcess();
	// Sets the CPU registers to the new PCB registers
	if (!readyQueue.empty())
	{
		CPU->set_registers(readyQueue.getProcess()->get_registers());
#ifdef DEBUG
		printf("Correctly swapped processes!!!!\n");
#endif // DEBUG
	}
}


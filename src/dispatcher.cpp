#include "dispatcher.hpp"
#include "PriorityQueue.h"

void Dispatcher::dispatch(cpu* CPU, PCB* cProcess, int cCycle, int maxCycle) {
	// If the readyQueue is not empty
	if (!readyQueue.empty() &&( (cProcess->get_status() == status::WAITING) || (cCycle >= maxCycle)))
	{
		switchOut(CPU, cProcess);   //  Moves current process to WaitingQueue
		switchIn(CPU);			  //  Removes First Process and gives next process to CPU
	}
}


void Dispatcher::switchOut(cpu* CPU, PCB* cProcess) {
	cProcess->set_registers(CPU->registers);
	//PCB* exitingProcess = readyQueue.getProcess();  //  Creates a pointer to the Current Running Process, calling it "exitingProcess"
	//waitingQueue.Q.push(exitingProcess);			//  Moves the exiting process into the Waiting Queue
}


void Dispatcher::switchIn(cpu* CPU) {
	// Removes the Current Running Process, so the next one will be the new active process
	readyQueue.removeProcess();
	// Sets the CPU registers to the new PCB registers
	if (!readyQueue.empty())
	{
		CPU->set_registers(readyQueue.getProcess()->get_registers());
	}
	//  Assigns the new First Process in the Ready Queue to the current Process
	//  in the CPU
	//CPU.CurrentProcess = readyQueue.getProcess();
}
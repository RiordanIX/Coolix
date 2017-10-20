#pragma once
#include <deque>
#include "pcb.hpp"

class FIFO
{
public:
	std::deque<PCB*> ready;
	FIFO();
	~FIFO();
	PCB* GetProcess();
	void AddProcess(PCB* procc);
	void RemoveProcess();
	int Size();
};
FIFO readyQueue, waitingQueue, terminatedQueue, newQueue;

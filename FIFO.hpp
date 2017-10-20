#pragma once
#include <deque>
#include "PCB.h"

class FIFO
{
public:
	std::deque<PCB*> ready;
	FIFO();
	~FIFO();

	PCB* getProcess();
	void addProcess(PCB* procc);
	void removeProcess();
	int Size(){ return ready.size()};
};

FIFO readyQueue, waitingQueue, terminatedQueue, newQueue;

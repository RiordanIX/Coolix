#pragma once
#include <queue>  
#include "PCB.h"
class PriorityQueue
{
public:
	struct LessThanByP
	{
		bool operator()(const PCB * lhs, const PCB * rhs) const
		{
			return lhs->P > rhs->P;
		}
	};
	std::priority_queue<PCB*, std::deque<PCB *>,LessThanByP> Q; // Means Queue
	PriorityQueue();
	~PriorityQueue();
	PCB* getProcess();
	void addProcess(PCB* procc);
	void removerProcess();
    
    static PriorityQueue readyQueue, waitingQueue, terminatedQueue, newQueue;
};



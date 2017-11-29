#pragma once
#include <queue>
#include <deque>
#include "pcb.hpp"

class PriorityQueue
{
public:
	struct LessThanByP
	{
		bool operator()(PCB *const lhs, PCB * const rhs) const
		{
			return lhs->get_priority() > rhs->get_priority();
		}
	};
	//std::priority_queue<PCB*, std::deque<PCB *>,LessThanByP> Q; // Means Queue
	std::deque<PCB*> Q;
	PriorityQueue();
	~PriorityQueue();

	PCB* getProcess();
	void addProcess(PCB* procc);
	void removeProcess();
	int size();
	bool empty() { return Q.empty(); };
};



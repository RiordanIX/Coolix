#include "stdafx.h"
#include "PriorityQueue.h"

PriorityQueue::PriorityQueue()//initialize priority queue
{
	Q = std::priority_queue<PCB*, std::deque<PCB*>, LessThanByP>();
}
PriorityQueue::~PriorityQueue()//so clears piriority queue table, however we never using this method
{
	while (Q.empty == false)
	{
		Q.pop();
	}
}
PCB* PriorityQueue::getProcess()
{
	return Q.top();
}
void PriorityQueue::addProcess(PCB* procc)
{
	Q.push(procc);
}
void PriorityQueue::removerProcess()
{
	Q.pop();
}


#include "stdafx.h"
#include "PriorityQueue.h"



PriorityQueue::PriorityQueue()//initialize priority queue
{
	Q = std::priority_queue<PCB*, std::deque<PCB*>, LessThanByP>();
}
PriorityQueue::~PriorityQueue()//reinitializing priority queue to point to empty queue
{
	Q = std::priority_queue<PCB*, std::deque<PCB*>, LessThanByP>();
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


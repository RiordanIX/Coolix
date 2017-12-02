#include "PriorityQueue.hpp"

//initialize priority queue
PriorityQueue::PriorityQueue() {
	Q = std::priority_queue<PCB*, std::deque<PCB*>, LessThanByP>();
	fifo = std::deque<PCB*>();
}


PriorityQueue::~PriorityQueue() {
	while (Q.empty() == false)
	{
		if (sortby._Equal("P") || sortby._Equal("SJF"))
		{
			Q.pop();
		}
		else
		{
			fifo.pop_front();
		}
	}
}


PCB* PriorityQueue::getProcess() {
		if (sortby._Equal("P") || sortby._Equal("SJF"))
		{
			return Q.top();
		}
		else
		{
			return fifo.front();
		}
}


void PriorityQueue::addProcess(PCB* procc) {
	if (sortby._Equal("P") || sortby._Equal("SJF"))
	{
		Q.push(procc);
	}
	else
	{
		fifo.push_back(procc);
	}
}
void PriorityQueue::addProcessfront(PCB* procc) {
	if (sortby._Equal("P") || sortby._Equal("SJF"))
	{
		Q.push(procc);
	}
	else
	{
		fifo.push_front(procc);
	}
}

void PriorityQueue::removeProcess() {
	if (sortby._Equal("P") || sortby._Equal("SJF"))
	{
		Q.pop();
	}
	else
	{
		fifo.pop_front();
	}
}


int PriorityQueue::size() {

	if (sortby._Equal("P") || sortby._Equal("SJF"))
	{
		return Q.size();
	}
	else
	{
		return fifo.size();
	}
}

bool PriorityQueue::empty()
{
	if (sortby._Equal("P") || sortby._Equal("SJF"))
	{
		 return Q.empty();
	}
	else
	{
		return fifo.empty();
	}
}


// GLOBAL VARIABLES
PriorityQueue readyQueue;


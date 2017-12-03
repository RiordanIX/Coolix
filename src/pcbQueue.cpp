#include "pcbQueue.hpp"

//initialize priority queue
pcbQueue::pcbQueue() {
	Q = std::priority_queue<PCB*, std::deque<PCB*>, LessThanByP>();
	fifo = std::deque<PCB*>();
}


pcbQueue::~pcbQueue() {
	while (Q.empty() == false)
	{
		if (sortby.compare("P") == 0 || sortby.compare("SJF") == 0)
		{
			Q.pop();
		}
		else
		{
			fifo.pop_front();
		}
	}
}


PCB* pcbQueue::getProcess() {
		if (sortby.compare("P") == 0|| sortby.compare("SJF") == 0)
		{
			return Q.top();
		}
		else
		{
			return fifo.front();
		}
}


void pcbQueue::addProcess(PCB* procc) {
	if (sortby.compare("P") == 0 || sortby.compare("SJF") == 0)
	{
		Q.push(procc);
	}
	else
	{
		fifo.push_back(procc);
	}
}
void pcbQueue::addProcessfront(PCB* procc) {
	if (sortby.compare("P") == 0 || sortby.compare("SJF") == 0)
	{
		Q.push(procc);
	}
	else
	{
		fifo.push_front(procc);
	}
}

void pcbQueue::removeProcess() {
	if (sortby.compare("P") == 0 || sortby.compare("SJF") == 0)
	{
		Q.pop();
	}
	else
	{
		fifo.pop_front();
	}
}


int pcbQueue::size() {

	if (sortby.compare("P") == 0 || sortby.compare("SJF") == 0)
	{
		return Q.size();
	}
	else
	{
		return fifo.size();
	}
}

bool pcbQueue::empty()
{
	if (sortby.compare("P") == 0 || sortby.compare("SJF") == 0)
	{
		 return Q.empty();
	}
	else
	{
		return fifo.empty();
	}
}


// GLOBAL VARIABLES
pcbQueue readyQueue;


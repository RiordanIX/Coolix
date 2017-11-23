#include "PriorityQueue.hpp"

//initialize priority queue
PriorityQueue::PriorityQueue() {
	Q = std::priority_queue<PCB*, std::deque<PCB*>, LessThanByP>();
}


PriorityQueue::~PriorityQueue() {
	while (Q.empty() == false)
	{
		Q.pop();
	}
}


PCB* PriorityQueue::getProcess() {
	if (Q.empty()) {
		return nullptr;
	}
	else {
		return Q.top();
	}
}


void PriorityQueue::addProcess(PCB* procc) {
	Q.push(procc);
}


void PriorityQueue::removeProcess() {
	Q.pop();
}


int PriorityQueue::size() {
	return Q.size();
}


// GLOBAL VARIABLES
PriorityQueue readyQueue, waitingQueue, terminatedQueue, newQueue;


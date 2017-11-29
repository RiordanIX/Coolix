#include "PriorityQueue.hpp"

//initialize priority queue
PriorityQueue::PriorityQueue() {
	//Q = std::priority_queue<PCB*, std::deque<PCB*>, LessThanByP>();
	Q = std::deque<PCB*>();
}


PriorityQueue::~PriorityQueue() {
	while (Q.empty() == false)
	{
		Q.pop_front();
	}
}


PCB* PriorityQueue::getProcess() {
	if (Q.empty()) {
		return nullptr;
	}
	else {
		return Q.front();
	}
}


void PriorityQueue::addProcess(PCB* procc) {
	Q.push_back(procc);
}


void PriorityQueue::removeProcess() {
	Q.pop_front();
}


int PriorityQueue::size() {
	return Q.size();
}


// GLOBAL VARIABLES
PriorityQueue readyQueue, waitingQueue, terminatedQueue, newQueue;


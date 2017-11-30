#include "FIFO.hpp"

//initialize queue
FIFO::FIFO(){
	Q = std::deque<PCB*>();
}

FIFO::~FIFO() {
	Q.clear();
}

PCB* FIFO::getProcess() {
	return Q.front();
}

void FIFO::addProcess(PCB* procc) {
	Q.push_back(procc);
}

void FIFO::removeProcess() {
	Q.pop_front();
}
bool FIFO::empty()
{
	return Q.empty();
}
FIFO waitingQueue, terminatedQueue, newQueue;
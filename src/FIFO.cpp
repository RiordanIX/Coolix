#include "stdafx.h"
#include "FIFO.h"

//initialize queue
FIFO::FIFO(){
	ready = std::deque<PCB*>();
}

FIFO::~FIFO() {
	ready.clear();
}

PCB* FIFO::getProcess() {
	return ready.front();
}

void FIFO::addProcess(PCB* procc) {
	ready.push_back(procc);
}

void FIFO::removeProcess() {
	ready.pop_front();
}
int FIFO::Size()
{
	return ready.size();
}

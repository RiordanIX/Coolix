#include "stdafx.h"
#include "FIFO.h"


FIFO::FIFO()//initialize queue
{
	ready = std::deque<PCB*>();
}
FIFO::~FIFO()
{
	ready.clear();
}
PCB* FIFO::GetProcess() 
{
	return ready.front();
}
void FIFO::AddProcess(PCB* procc) 
{
	ready.push_back(procc);
}
void FIFO::RemoverProcess() 
{
	ready.pop_back();
}

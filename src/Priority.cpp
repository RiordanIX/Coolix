#include "stdafx.h"
#include "Priority.h"



Priority::Priority()//initialize priority queue
{
	ready = std::priority_queue<PCB*, std::deque<PCB*>, LessThanByP>();
}
Priority::~Priority()//reinitializing priority queue to point to empty queue
{
	ready = std::priority_queue<PCB*, std::deque<PCB*>, LessThanByP>();
}
PCB* Priority::GetProcess()
{
	return ready.top();
}
void Priority::AddProcess(PCB* procc)
{
	ready.push(procc);
}
void Priority::RemoverProcess()
{
	ready.pop();
}


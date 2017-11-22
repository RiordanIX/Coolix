#pragma once
#include <iostream>
#include "pcb.hpp"
#include "PriorityQueue.h"

class ShortTermScheduler
{
public:
    ShortTermScheduler();
    ~ShortTermScheduler();
	//move io bond process from ready to wait queue if io is busy
	void ReadyToWait();
	//move io bond process from ready to wait queue if io is busy
	void WaitToReady();
private:
	//check to see if resource process needs is not being used
	bool HeldResource(PCB* pcb);
};

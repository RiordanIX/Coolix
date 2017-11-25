#pragma once
#include <iostream>
#include "pcb.hpp"
#include "CpuPool.hpp"
#include "PriorityQueue.hpp"
#include "mmu.hpp"

class ShortTermScheduler
{
public:
    ShortTermScheduler();
    ~ShortTermScheduler();
	//move io bond process from ready to wait queue if io is busy
	void ReadyToWait();
	//move io bond process from wait to ready queue if io is not busy or mmu is finished moving process pages
	void WaitToReady();
	//move to running process to wait queue if process page faults
	static void RunningToWait(PCB* pcb);
};

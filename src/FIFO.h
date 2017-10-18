#pragma once
#include <deque>
#include "PCB.h"

class FIFO
{
public:
	std::deque<PCB*> ready;
	FIFO();
	~FIFO();
	PCB* GetProcess();
	void AddProcess(PCB* procc);
	void RemoverProcess();
};


#pragma once
#include <deque>
#include "PCB.hpp"

class FIFO
{
public:
	std::deque<PCB*> Q;
	FIFO();
	~FIFO();

	PCB* getProcess();
	void addProcess(PCB* procc);
	void removeProcess();
	int size() { return Q.size(); }
	bool empty();
};



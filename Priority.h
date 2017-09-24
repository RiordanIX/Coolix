#pragma once
#include <queue>  
#include "PCB.h"
class Priority
{
public:
	struct LessThanByP
	{
		bool operator()(const PCB * lhs, const PCB * rhs) const
		{
			return lhs->P > rhs->P;
		}
	};
	std::priority_queue<PCB*, std::deque<PCB *>,LessThanByP> ready;
	Priority();
	~Priority();
	PCB* GetProcess();
	void AddProcess(PCB* procc);
	void RemoverProcess();
};


#pragma once
#include <queue>
#include <deque>
#include "pcb.hpp"
#include "mutex.hpp"

extern std::string sortby;

class PriorityQueue
{
public:
	struct LessThanByP
	{
		bool operator()(PCB *const lhs, PCB * const rhs) const
		{
			if (sortby._Equal("P"))
			{
				return lhs->get_priority() > rhs->get_priority();
			}
			else if(sortby._Equal("SJF"))
			{
				return lhs->get_inp_address() > rhs->get_inp_address();
			}
			else
			{
				return lhs->get_pid() > rhs->get_pid();
			}
		}
	};
	Mutex mutex;
	std::priority_queue<PCB*, std::deque<PCB *>,LessThanByP> Q; // Means Queue
	std::deque<PCB*> fifo;
	PriorityQueue();
	~PriorityQueue();

	PCB* getProcess();
	void addProcess(PCB* procc);
	void addProcessfront(PCB* procc);
	void removeProcess();
	int size();
	bool empty();
	void setLock() { while (mutex == LOCK) {} mutex = LOCK; }
	void freeLock() { mutex = FREE; }
	Mutex getLock() { return mutex; }
};



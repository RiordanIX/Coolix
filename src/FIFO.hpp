#pragma once
#include <deque>
#include "PCB.hpp"
#include "mutex.hpp"

class FIFO
{
public:
	std::deque<PCB*> Q;
	FIFO();
	~FIFO();
	Mutex mutex;
	PCB* getProcess();
	void addProcess(PCB* procc);
	void removeProcess();
	int size() { return Q.size(); }
	bool empty();
	void setLock() 
	{ 
		while (mutex == LOCK) {}
		mutex = LOCK; 
	}
	void freeLock() { mutex = FREE; }
	Mutex getLock() { return mutex; }
};



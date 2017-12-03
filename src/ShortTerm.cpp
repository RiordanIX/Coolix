#include "ShortTerm.hpp"
#include "FIFO.hpp"

extern std::vector<PCB> process_list;
extern pcbQueue readyQueue;
extern FIFO waitingQueue;

void ShortTermScheduler::RunningToWait(PCB* pcb)
{
		/*If process is running and page faults */
		//places ready queue process into wait queue
		//set pcb status to waiting
		//while (mtx.try_lock() == false) {}
	
		if (pcb->get_waitformmu() == true)
		{	
			waitingQueue.setLock();
			waitingQueue.addProcess(pcb);
			waitingQueue.freeLock();
		}
		
		
	
}
void ShortTermScheduler::ReadyToWait()
{
	/*how to pop ready queue pointer if process goes to wait?*/
	readyQueue.setLock();
	if (readyQueue.size() > 0 )
	{
		//places ready queue process into wait queue
		//set pcb status to waiting
		if (Hardware::GetResourceLock(readyQueue.getProcess()->get_resource_status()) == LOCK)
		{
			readyQueue.getProcess()->set_status(status::WAITING);
			readyQueue.getProcess()->set_wait_start_clock();
			waitingQueue.setLock();
			waitingQueue.addProcess(readyQueue.getProcess());
			readyQueue.removeProcess();
			waitingQueue.freeLock();
		}
	}
	readyQueue.freeLock();
}
bool waitingQueueSize()
{
	waitingQueue.setLock();
	bool there = false;
	if (waitingQueue.size() > 0)
	{
		there = true;
	}
	waitingQueue.freeLock();
	return there;
}
bool processNotHardwareMmuWaiting()
{
	waitingQueue.setLock();
	bool there = false;
	if (waitingQueue.size() > 0)
	{
		if (Hardware::GetResourceLock(waitingQueue.getProcess()->get_resource_status()) == FREE
			&& waitingQueue.getProcess()->get_waitformmu() == false)
		{
			there = true;
		}
	}
	waitingQueue.freeLock();
	return there;
}
void ShortTermScheduler::WaitToReady()
{
	/*how to pop wait queue pointer if process goes to ready?*/
	//set pcb status to ready
	//check to see if resouce is free and process is not waiting for mmu
	if (waitingQueueSize())
	{
		if (processNotHardwareMmuWaiting())
		{
			readyQueue.setLock();
			waitingQueue.setLock();
			readyQueue.addProcessfront(waitingQueue.getProcess());
			waitingQueue.getProcess()->set_status(status::READY);
			waitingQueue.removeProcess();
			readyQueue.freeLock();
			waitingQueue.freeLock();
		}
	}
}


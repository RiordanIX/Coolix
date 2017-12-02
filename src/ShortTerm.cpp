#include "ShortTerm.hpp"
#include "FIFO.hpp"
#include <mutex>

extern std::vector<PCB> process_list;
extern PriorityQueue readyQueue;
extern FIFO waitingQueue;
extern int readyQueueLock;
extern int waitQueueLock;
;
ShortTermScheduler::ShortTermScheduler()
{
  
}

ShortTermScheduler::~ShortTermScheduler()
{

}
void ShortTermScheduler::RunningToWait(PCB* pcb)
{
		/*If process is running and page faults */
		//places ready queue process into wait queue
		//set pcb status to waiting
		//while (mtx.try_lock() == false) {}
	
		if (pcb->get_waitformmu() == true)
		{	
			while (waitQueueLock == 1) { printf("RunningToWait"); }
			waitQueueLock = 1;
			waitingQueue.addProcess(pcb);
			waitQueueLock = 0;
		}
		
		
	
}
void ShortTermScheduler::ReadyToWait()
{
	/*how to pop ready queue pointer if process goes to wait?*/
	while (readyQueueLock == 1) { printf("readyQ"); }
	readyQueueLock = 1;
	if (readyQueue.size() > 0 )
	{
		//places ready queue process into wait queue
		//set pcb status to waiting
		if (Hardware::GetResourceLock(readyQueue.getProcess()->get_resource_status()) == LOCK)
		{
			readyQueue.getProcess()->set_status(status::WAITING);
			while (waitQueueLock == 1) { printf("waitingQ"); }
			waitQueueLock = 1;
			waitingQueue.addProcess(readyQueue.getProcess());
			readyQueue.removeProcess();
			waitQueueLock = 0;
		}
	}
	readyQueueLock = 0;
}
bool waitingQueueSize()
{
	while (waitQueueLock == 1) { printf("waitingQ"); }
	waitQueueLock = 1;
	bool there = false;
	if (waitingQueue.size() > 0)
	{
		there = true;
	}
	waitQueueLock = 0;
	return there;
}
bool processNotHardwareMmuWaiting()
{
	while (waitQueueLock == 1) { printf("waitingQ"); }
	waitQueueLock = 1;
	bool there = false;
	if (waitingQueue.size() > 0)
	{
		if (Hardware::GetResourceLock(waitingQueue.getProcess()->get_resource_status()) == FREE
			&& waitingQueue.getProcess()->get_waitformmu() == false)
		{
			there = true;
		}
	}
	waitQueueLock = 0;
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
			while (readyQueueLock == 1) { printf("readyQ"); }
			readyQueueLock = 1;
			while (waitQueueLock == 1) { printf("waitingQ"); }
			waitQueueLock = 1;
			readyQueue.addProcessfront(waitingQueue.getProcess());
			waitingQueue.getProcess()->set_status(status::READY);
			waitingQueue.removeProcess();
			readyQueueLock = 0;
			waitQueueLock = 0;
		}
	}
}


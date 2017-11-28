#include "ShortTerm.hpp"
#include <mutex>

extern std::vector<PCB> process_list;
extern PriorityQueue readyQueue, waitingQueue;
extern int waitQueueLock;

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
			while (waitQueueLock == 1) {}
			waitQueueLock = 1;
			waitingQueue.addProcess(pcb);
			waitQueueLock = 0;
		}
		
	
}
void ShortTermScheduler::ReadyToWait()
{
	/*how to pop ready queue pointer if process goes to wait?*/
	
	if (readyQueue.size() > 0 )
	{
		//places ready queue process into wait queue
		//set pcb status to waiting
		if (Hardware::GetResourceLock(readyQueue.getProcess()->get_resource_status()) == LOCK)
		{
			readyQueue.getProcess()->set_status(status::WAITING);
			while (waitQueueLock == 1) {}
			waitQueueLock = 1;
			waitingQueue.addProcess(readyQueue.getProcess());
			waitQueueLock = 0;
			
			readyQueue.removeProcess();
			
		}
	}
}
void ShortTermScheduler::WaitToReady()
{
	/*how to pop wait queue pointer if process goes to ready?*/
	//set pcb status to ready
	//check to see if resouce is free and process is not waiting for mmu
	if (waitingQueue.size() > 0 )
	{
		while (waitQueueLock == 1) {}
		waitQueueLock = 1;
		if (Hardware::GetResourceLock(waitingQueue.getProcess()->get_resource_status()) == FREE
			&& waitingQueue.getProcess()->get_waitformmu() == false)
		{   
			readyQueue.addProcess(waitingQueue.getProcess());
			waitingQueue.getProcess()->set_status(status::READY);
			waitingQueue.removeProcess();
		}
		waitQueueLock = 0;
		

	}

}

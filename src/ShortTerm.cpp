#include "ShortTerm.hpp"

extern std::vector<PCB> process_list;
extern PriorityQueue readyQueue, waitingQueue;

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
		if (pcb->get_waitformmu() == true)
     	{
			readyQueue.getProcess()->set_status(status::WAITING);
			waitingQueue.addProcess(readyQueue.getProcess());
			readyQueue.removeProcess();
		}
}
void ShortTermScheduler::ReadyToWait()
{
	/*how to pop ready queue pointer if process goes to wait?*/
	
	if (readyQueue.size() > 0)
	{
		//places ready queue process into wait queue
		//set pcb status to waiting
		if (HeldResource(readyQueue.getProcess()) == true)
		{
			readyQueue.getProcess()->set_status(status::WAITING);
			waitingQueue.addProcess(readyQueue.getProcess());
			readyQueue.removeProcess();
		}
	}
}
void ShortTermScheduler::WaitToReady()
{
	/*how to pop wait queue pointer if process goes to ready?*/
	//set pcb status to ready
	if (HeldResource(waitingQueue.getProcess()) == false && waitingQueue.getProcess()->get_waitformmu() == false)
	{
		readyQueue.addProcess(waitingQueue.getProcess());
		waitingQueue.getProcess()->set_status(status::READY);
		waitingQueue.removeProcess();
	}

}
//returns true or false if I/O resource is being used by a running process
bool ShortTermScheduler::HeldResource(PCB* pcb)
{
	
	bool held = false;
	for (auto it = process_list.begin(); it != process_list.end(); ++it)//loops through process list of pcbs
	{
		if ((*it).get_status() == status::RUNNING)//running process
		{
			if ((*it).get_resource_status() == pcb->get_resource_status())//if resource are the same
			{
				held = true;
				break;
			}
		}

	}
	return held;
}

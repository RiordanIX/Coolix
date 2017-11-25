#include "LongTerm.hpp"

// GLOBAL VARIABLES!!!
extern std::vector<PCB> process_list;
extern PriorityQueue readyQueue, waitingQueue, terminatedQueue, newQueue;
extern Disk DISK;
extern mmu MMU;

LongTerm::LongTerm() {
	ReadySize = 0;
	MaxAddress = 0;
}


LongTerm::~LongTerm() { }


void LongTerm::loadProcess() {
	PCB *pcb = newQueue.getProcess();
	// Load first 4 pages into RAM
	for (int i = 0; i < 4; ++i) {
		if (MMU.processDiskToRam(pcb, i) )
			continue;
		else
			return;
	}
	pcb->set_status(status::READY);
	readyQueue.addProcess(pcb);
	newQueue.removeProcess();
}


std::vector<LongTerm::EmptySpace> LongTerm::GetOpenSpaces() {
	std::vector<EmptySpace> ess;
	used.clear();
	// unsigned int prestart = 0;
	unsigned int preend = 0;
	ReadySize = 0;
	MaxAddress = 0;
	if(readyQueue.size() > 0)//check if ready queue has any processes loaded
	{
		//since we cannot iterate through a priority_queue, therefore, we go through a process list to
		//determine which process is in ready queue
		for (unsigned int x = 0; x < process_list.size(); x++)
		{
			//process in ready queue have a status of READY
			if (process_list[x].get_status() == status::READY || process_list[x].get_status() == status::WAITING)
			{
				used.push_back(Used(process_list[x].get_ram_address(), process_list[x].get_ram_address()+(process_list[x].get_end_address())));
				ReadySize += (process_list[x].get_end_address());
				MaxAddress = ReadySize;
			}
		}
		if (used.size() > 0)
		{
			std::sort(used.begin(), used.end(), SortUsed);
			preend = 0;
			for (unsigned int x = 0; x < used.size(); x++)
			{
				//compare next process in ready status to see if there is a memory hole
				if (preend < (used[x].Start))
				{
					//check to see if this space is really empty
					if (CheckEmpty(EmptySpace(preend, used[x].Start), used))
					{
						//if there is a memory hole than it stores its start and end address into a vector
						ess.push_back(EmptySpace(preend, used[x].Start));
					}
				}
				preend = used[x].End;
			}
		}
	}
	return ess;
}
//check if this range is empty within Ram
bool LongTerm::CheckEmpty(EmptySpace es, std::vector<Used> used)
{
	bool there = true;
	for (unsigned int x = 0; x < used.size(); x++)
	{
		if (es.Sadd >= used[x].Start &&  es.Isize <= used[x].End)
		{
			there = false; break;
		}
		else if (es.Sadd < used[x].Start && used[x].Start < es.Isize)
		{
			there = false; break;
		}
	}
	return there;
}
//check to see if another process that require same resource is in running state or not
bool LongTerm::CheckResource(resourceType RT)
{
	bool there = true;
	for (unsigned int x = 0; x < process_list.size(); x++)
	{
		//process in ready queue have a status of READY
		if (process_list[x].get_status() == status::RUNNING)
		{
			if (process_list[x].get_resource_status() == RT)
			{
				there = false; break;
			}
		}
	}
	return there;
}

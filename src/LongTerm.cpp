#include "stdafx.h"
#include "LongTerm.h"


LongTerm::LongTerm()
{
	ReadySize = 0;
	MaxAddress = 0;
}


LongTerm::~LongTerm()
{
}

void LongTerm::DiskToRam()
{
	std::vector<EmptySpace> ess1 = GetOpenSpaces();//gets vectory of addresses and size for memory holes
	int spotNotfound = 0;//use this variable to determine if process fits in memory hole or not
	if (ReadySize < DEFAULT_RAM)//checks if ram still have space
	{
		for (int x = 0; x < process_list.size(); x++)
		{
			//store process with a status of NEW 
			if (process_list[x].get_resource_status() == NEW)
			{
				spotNotfound++;
				for (int i = 0; i < ess1.size(); i++)//loop through empty spaces to see if anything fits
				{
					if ((ess1[i].Isize - ess1[i].Sadd) >= (process_list[x].get_end_address() - process_list[x].get_ram_address()))
					{
						ReadySize += (process_list[x].get_end_address() - process_list[x].get_ram_address());
						if (ReadySize >= DEFAULT_RAM)//if ram is full than break the loop before allocating space in ram 
						{
							break;
						}
						Ram::allocate(ess1[i].Sadd, process_list[x].get_disk_address());
						/*insert into ready queue here*/
						PriorityQueue::readyQueue.addProcess(&process_list[x]);
						process_list[x].set_status(READY);//update pcb
						
						spotNotfound--;
						break;
					}
				}
				if (ReadySize >= DEFAULT_RAM)//minus the previous size and so we look for another process that would fit
				{
					ReadySize = ReadySize - (process_list[x].get_end_address() - process_list[x].get_ram_address());
					spotNotfound--;
				}
				else if (spotNotfound > 0)//if process is not placed in memory hole than place it at the end of max end address in ready state.
				{
					/*insert in ready queue here*/
					ReadySize += (process_list[x].get_end_address() - process_list[x].get_ram_address());
					if (ReadySize <= DEFAULT_RAM)//if ram is full than break the loop before allocating space in ram 
					{
						Ram::allocate(MaxAddress, process_list[x].get_disk_address());
						MaxAddress = process_list[x].get_end_address();
						/*insert into ready queue here*/
						PriorityQueue::readyQueue.addProcess(&process_list[x]);
						process_list[x].set_status(READY);//update pcb
					}
					else
					{
						ReadySize = ReadySize - (process_list[x].get_end_address() - process_list[x].get_ram_address());
					}

				}
			}
		}
	}
}
void LongTerm::ReadyToWait()
{
	/*how to pop ready queue pointer if process goes to wait?*/
}

void LongTerm::WaitToReady()
{
	/*how to pop wait queue pointer if process goes to ready?*/
}

std::vector<LongTerm::EmptySpace> LongTerm::GetOpenSpaces()
{
	std::vector<EmptySpace> ess;
	std::vector<Used> used;
	int prestart = 0;
	int preend = 0;
	ReadySize = 0;
	MaxAddress = 0;
	if(PriorityQueue::readyQueue.Size() > 0)//check if ready queue has any processes loaded
	{ 
		//since we cannot iterate through a priority_queue, therefore, we go through a process list to 
		//determine which process is in ready queue
		for (int x = 0; x < process_list.size(); x++)
		{
			//process in ready queue have a status of READY 
			if (process_list[x].get_resource_status() == READY)
			{
				if (MaxAddress < process_list[x].get_end_address())
				{
					MaxAddress = process_list[x].get_end_address();
				}
				used.push_back(Used(process_list[x].get_ram_address(),process_list[x].get_end_address()));
				ReadySize += (process_list[x].get_end_address() - process_list[x].get_ram_address());
			}
		}
		if (used.size() > 0)
		{
			for (int x = 0; x < used.size(); x++)
			{
				//compare next process in ready status to see if there is a memory hole
				if (preend != 0)
				{
					if (preend < (used[x].Start))
					{
						//check to see if this space is really empty
						if (CheckEmpty(EmptySpace(preend, used[x].Start), used))
						{
							//if there is a memory hole than it stores its start and end address into a vector
							ess.push_back(EmptySpace(preend, used[x].Start));
						}
					}
				}
				prestart = used[x].Start;
				preend = used[x].End;
			}
		}
	}
	return ess;
}
//check if this range is empty within ready queue
bool LongTerm::CheckEmpty(EmptySpace es, std::vector<Used> used)
{
	bool there = true;
	for (int x = 0; x < used.size(); x++)
	{
		if (es.Sadd >= used[x].Start && used[x].End >= es.Isize)
		{
			there = false; break;
		}
		else if (es.Sadd < used[x].Start && used[x].Start < es.Isize)
		{
			there = false; break;
		}
		else if (es.Sadd >= used[x].Start && used[x].End < es.Isize)
		{
			there = false; break;
		}
	}
	return there;
}
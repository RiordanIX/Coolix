#include "LongTerm.h"

// GLOBAL VARIABLES!!!
extern std::vector<PCB> process_list;
extern PriorityQueue readyQueue, waitingQueue, terminatedQueue, newQueue;
extern Disk DISK;
extern Ram MEM;

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
	std::string str;
	std::vector<EmptySpace> ess1 = GetOpenSpaces();//gets vector of addresses and size for memory holes
	int spotNotfound = 0;//use this variable to determine if process fits in memory hole or not
	if (ReadySize < DEFAULT_RAM)//checks if ram still have space
	{
		for (unsigned int x = 0; x < process_list.size(); x++)
		{
			//store process with a status of NEW
			if (process_list[x].get_status() == status::NEW)
			{
				spotNotfound++;
				for (unsigned int i = 0; i < ess1.size(); i++)//loop through empty spaces to see if anything fits
				{
					if ((ess1[i].Isize - ess1[i].Sadd) >= (process_list[x].get_end_address()))
					{
						ReadySize += ((process_list[x].get_end_address()));
						if (ReadySize >= DEFAULT_RAM)//if ram is full than break the loop before allocating space in ram
						{
							break;
						}
						MEM.allocate_chunk(ess1[i].Sadd, DISK.read_instruction_chunk(process_list[x].get_disk_address(), (process_list[x].get_end_address())));
						
						std::printf("Allocated to RAM Process id:%u", process_list[x].get_pid());
						process_list[x].set_ram_address(ess1[i].Sadd);
						/*insert into ready queue here*/
						readyQueue.addProcess(&process_list[x]);
						process_list[x].set_status(READY);//update pcb

						spotNotfound--;
						break;
					}
				}
				if (ReadySize >= DEFAULT_RAM)//minus the previous size and so we look for another process that would fit
				{
					ReadySize = ReadySize - ((process_list[x].get_end_address()));
					spotNotfound--;
				}
				else if (spotNotfound > 0)//if process is not placed in memory hole than place it at the end of max end address in ready state.
				{
					spotNotfound = 0;
					/*insert in ready queue here*/
					
					ReadySize += (process_list[x].get_end_address());
					if (ReadySize <= DEFAULT_RAM)//if ram is full than break the loop before allocating space in ram
					{
						process_list[x].set_ram_address(MaxAddress);
						MEM.allocate_chunk(MaxAddress, DISK.read_instruction_chunk(process_list[x].get_disk_address(), process_list[x].get_end_address()));
						MaxAddress = MaxAddress + (process_list[x].get_end_address());//not sure about this
						/*insert into ready queue here*/
						std::printf("Allocated to RAM Process id:%u", process_list[x].get_pid());
						process_list[x].set_status(status::READY);//update pcb
						readyQueue.addProcess(&process_list[x]);
						if (ReadySize == DEFAULT_RAM)
						{
							break;
						}
					}
					else
					{
						ReadySize = ReadySize - (process_list[x].get_inp_address() * 4);
					}

				}
			}
		}
	}
}
void LongTerm::ReadyToWait()
{
	/*how to pop ready queue pointer if process goes to wait?*/
	std::vector<PCB*> hold;//variable is used to refill queue b/c pop is the only way to iterate through priority queue
	while (readyQueue.size() > 0)
	{
		if (readyQueue.getProcess()->get_resource_status() != resourceType::NONE)
		{
			if (CheckResource( readyQueue.getProcess()->get_resource_status() ) == false )//need a getter for resourse that returns if true or false if resource being held at the moment
			{//if resource being held then pop this pcb from ready queue and push it into wait queue
				waitingQueue.addProcess(readyQueue.getProcess());
				readyQueue.getProcess()->set_status(status::WAITING);//update pcb
				readyQueue.removeProcess();
			}
		}
			hold.push_back(readyQueue.getProcess());
			readyQueue.removeProcess();

	}
	while (hold.empty()==false)//refill ready queue
	{
		readyQueue.addProcess(hold.back());
		hold.pop_back();
	}
}

void LongTerm::WaitToReady()
{
	/*how to pop wait queue pointer if process goes to ready?*/
	std::vector<PCB*> hold;//variable is used to refill queue b/c pop is the only way to iterate through priority queue
	while (waitingQueue.size() > 0)
	{
		if (waitingQueue.getProcess()->get_resource_status() != resourceType::NONE)
		{
			if (CheckResource(waitingQueue.getProcess()->get_resource_status()) == true)//need a getter for resourse that returns if true or false if resource being held at the moment
			{//if resource not being held then pop this pcb from wait queue and push it into ready queue
				readyQueue.addProcess(waitingQueue.getProcess());
				waitingQueue.getProcess()->set_status(status::READY);//update pcb
				waitingQueue.removeProcess();
			}
		}
		hold.push_back(waitingQueue.getProcess());
		waitingQueue.removeProcess();
	}
	while (hold.empty() == false)//refill wait queue
	{
		waitingQueue.addProcess(hold.back());
		hold.pop_back();
	}
}

std::vector<LongTerm::EmptySpace> LongTerm::GetOpenSpaces()
{
	std::vector<EmptySpace> ess;
	std::vector<Used> used;
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
			if (process_list[x].get_status() == status::READY)
			{
				if (MaxAddress < process_list[x].get_end_address())
				{
					MaxAddress = process_list[x].get_end_address();
				}
				used.push_back(Used(process_list[x].get_ram_address(), (process_list[x].get_end_address())));
				ReadySize += (process_list[x].get_end_address());
			}
		}
		if (used.size() > 0)
		{
			for (unsigned int x = 0; x < used.size(); x++)
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
				//prestart = used[x].Start;
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
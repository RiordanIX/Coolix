#include "CpuPool.hpp"

extern cpu CPU0, CPU1, CPU2, CPU3;
extern unsigned int cpucount;
std::vector<cpu*> cpuPool;

CPU_Pool::CPU_Pool()
{
	//add cpu's pointers into cpu pool vector
	CPU0.setId(0);
	CPU1.setId(1);
	CPU2.setId(2);
	CPU3.setId(3);
	cpuPool.push_back(&CPU0);
	cpuPool.push_back(&CPU1);
	cpuPool.push_back(&CPU2);
	cpuPool.push_back(&CPU3);
	while (cpucount < cpuPool.size())//sets user defined cpu count
	{
		cpuPool.pop_back();
	}
}
//Clear cpu registers that no longer running the same process
void CPU_Pool::clearCpu(unsigned int CpuID, unsigned int p_id)
{
	
	for (unsigned int x = 0; x < cpuPool.size(); x++)
	{
		if (cpuPool[x]->getProcess() != nullptr)
		{
			if (cpuPool[x]->getId() != CpuID
				&& cpuPool[x]->getProcess()->get_pid() == p_id)
			{
				cpuPool[x]->clear_registers();
				cpuPool[x]->setProcess(nullptr);
				cpuPool[x]->freeLock();
			}
		}
	}
	
}
cpu* CPU_Pool::FreeCPU()//check each cpu then determines which one is not running a process
{
	CPU = nullptr;
	for (unsigned int x = 0; x < cpuPool.size(); x++) {
		if (cpuPool[x]->getLock() == FREE)
		{
			CPU = cpuPool[x]; break;
		}
	}
	return CPU;
	
}
//Lock Resource by setting the mutex to lock
void Hardware::LockHardware(resourceType Rt)
{
	switch (Rt)
	{
		case DISK_IO:  IO1.SetLock(); break;
		case KEYBOARD: IO2.SetLock(); break;
		case SHMEM:    IO3.SetLock(); break;
		case STDOUT:   IO4.SetLock(); break;
		default:  break;
	}
}
//Free Resource by setting the mutex to free
void Hardware::FreeHardware(resourceType Rt)
{
	switch (Rt)
	{
		case DISK_IO:  IO1.FreeLock(); break;
		case KEYBOARD: IO2.FreeLock(); break;
		case SHMEM:    IO3.FreeLock(); break;
		case STDOUT:   IO4.FreeLock(); break;
		default:  break;
	}
}
//get if resource is being used or not
Mutex Hardware::GetResourceLock(resourceType Rt)
{
	switch (Rt)
	{
		case DISK_IO: return IO1.GetLock(); break;
		case KEYBOARD:return IO2.GetLock(); break;
		case SHMEM:   return IO3.GetLock(); break;
		case STDOUT:  return IO4.GetLock(); break;
		default:  return FREE; break;
	}
}

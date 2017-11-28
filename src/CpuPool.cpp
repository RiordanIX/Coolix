#include "CpuPool.hpp"

extern cpu CPU0, CPU1, CPU2, CPU3;

CPU_Pool::CPU_Pool()
{
}
cpu* CPU_Pool::FreeCPU()//check each cpu then determines which one is not running a process
{
	if (CPU0.currentProcess == nullptr)
	{
		return &CPU0;
	}
	else if (CPU0.currentProcess->get_status() != RUNNING)
	{
		return &CPU0;
	}
	else if (CPU1.currentProcess == nullptr)
	{
		return &CPU1;
	}
	else if (CPU1.currentProcess->get_status() != RUNNING)
	{
		return &CPU1;
	}
	else if (CPU2.currentProcess == nullptr)
	{
		return &CPU2;
	}
	else if (CPU2.currentProcess->get_status() != RUNNING)
	{
		return &CPU2;
	}
	else if (CPU3.currentProcess == nullptr)
	{
		return &CPU3;
	}
	else if (CPU3.currentProcess->get_status() != RUNNING)
	{
		return &CPU3;
	}
	else
	{
		return nullptr;
	}
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

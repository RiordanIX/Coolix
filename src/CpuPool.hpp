#include "cpu.hpp"
#include "mutex.hpp"

struct Hardware
{
private:
	resourceType Resource;
	Mutex mutex;
	void SetLock() { mutex = LOCK; }
	void FreeLock() { mutex = FREE; }
	Mutex GetLock() { return LOCK; }
public:
	Hardware(resourceType Rt, Mutex lock)
	{
		mutex = lock;
		Resource = Rt;
	}
	static void LockHardware(resourceType Rt);
	static void FreeHardware(resourceType Rt);
	static Mutex GetResourceLock(resourceType Rt);

};
class CPU_Pool
{
public:
	CPU_Pool();
	static void clearCpu(unsigned int CpuID, unsigned int p_id);
	cpu * FreeCPU();
private:
	cpu * CPU; //keep track of last cpu given out
};
static Hardware IO1 = Hardware(resourceType::DISK_IO, Mutex::FREE);
static Hardware IO2 = Hardware(resourceType::KEYBOARD, Mutex::FREE);
static Hardware IO3 = Hardware(resourceType::SHMEM, Mutex::FREE);
static Hardware IO4 = Hardware(resourceType::STDOUT, Mutex::FREE);
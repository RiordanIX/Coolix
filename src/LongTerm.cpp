#include "LongTerm.hpp"
#include <mutex>
#include "FIFO.hpp"
// GLOBAL VARIABLES!!!
extern std::vector<PCB> process_list;
extern pcbQueue readyQueue;
extern FIFO waitingQueue, terminatedQueue, newQueue;
extern Disk DISK;
extern mmu MMU;

//mutex for free frames add and drops
Mutex mutex;
void setLock() { while (mutex == LOCK) {} mutex = LOCK; }
void freeLock() { mutex = FREE; }

std::size_t LongTerm::FrameSize()
{
	setLock();
	size_t fframes = MMU.free_frame_count();
	freeLock();
	return fframes;
}
void LongTerm::loadProcess(PCB * pcb, std::size_t pagenumber)
{
	// Load 4 pages into RAM
	for (unsigned int i = pagenumber; i < (pagenumber + 4); ++i)
	{
		if (MMU.processDiskToRam(pcb, i) )
		{
			continue;
		}
		else
		{
			debug_printf("No frames are available%s", "\n");
			return;
		}
	}
	
	pcb->set_status(status::READY);
	readyQueue.addProcess(pcb);
}
bool Getframe(PCB* pcb, std::size_t pagenumber)
{
	setLock();
	bool there = false;
	there = MMU.processDiskToRam(pcb, pagenumber);
	freeLock();
	return there;
}
bool LongTerm::loadPage(PCB * pcb, std::size_t pagenumber)
{
	// Load 1 page into RAM
	if (!pcb->is_valid_page(pcb->get_lastRequestedPage()))
	{
		if (Getframe(pcb,pagenumber))
		{
			pcb->set_status(status::READY);
			return true;
		}
		else
		{
			debug_printf("No frames are available%s", "\n");
			return false;

		}
	}
	else
	{
		pcb->set_status(status::READY);
		return true;
	}
}
void LongTerm::DumpProcess(PCB * pcb)
{
	setLock();
	MMU.dumpProcess(pcb);
	freeLock();
}

void LongTerm::DumpFrame(PCB * pcb)
{
	setLock();
	MMU.dumpPage(pcb);
	freeLock();
}

int LongTerm::initialLoad()
{	
	int numberofjobs = 0;
	while (!newQueue.empty())
	{
		loadProcess(newQueue.getProcess(), 0);
		numberofjobs++;
		newQueue.removeProcess();
	}
	return numberofjobs;
}
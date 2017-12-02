#include "LongTerm.hpp"
#include <mutex>
#include "FIFO.hpp"
// GLOBAL VARIABLES!!!
extern std::vector<PCB> process_list;
extern PriorityQueue readyQueue;
extern FIFO waitingQueue, terminatedQueue, newQueue;
extern Disk DISK;
extern mmu MMU;
extern int FrameDump;

LongTerm::LongTerm() {
	ReadySize = 0;
	MaxAddress = 0;
}


LongTerm::~LongTerm() { }
std::size_t LongTerm::FrameSize()
{
	return MMU.free_frame_count();
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
	while (FrameDump == 1) { printf("framelock"); }
	FrameDump = 1;
	bool there = false;
	there = MMU.processDiskToRam(pcb, pagenumber);
	FrameDump = 0;
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
	while (FrameDump == 1) { printf("framelock"); }
	FrameDump = 1;
	MMU.dumpProcess(pcb);
	FrameDump = 0;
}

void LongTerm::DumpFrame(PCB * pcb)
{
	while (FrameDump == 1) { printf("framelock"); }
	FrameDump = 1;
	MMU.dumpPage(pcb);
	FrameDump = 0;
}



#include "LongTerm.hpp"
#include <mutex>
#include "FIFO.hpp"
// GLOBAL VARIABLES!!!
extern std::vector<PCB> process_list;
extern PriorityQueue readyQueue;
extern FIFO waitingQueue, terminatedQueue, newQueue;
extern Disk DISK;
extern mmu MMU;

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
bool LongTerm::loadPage(PCB * pcb, std::size_t pagenumber)
{
	// Load 1 page into RAM
	if (!pcb->is_valid_page(pcb->get_lastRequestedPage()))
	{
		if (MMU.processDiskToRam(pcb, pagenumber))
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
	MMU.dumpProcess(pcb);
}

void LongTerm::DumpFrame(PCB * pcb)
{
	MMU.dumpPage(pcb);
}



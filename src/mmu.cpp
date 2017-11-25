#include "mmu.hpp"

extern Ram MEM;
extern Disk DISK;

using std::size_t;

mmu::mmu()
{
	for(unsigned int i = 0; i < MEM.size() / PAGE_SIZE; i++)
		_freeFrames.push(i);
}
size_t mmu::getPhysicalAddress(PCB* pcb, size_t virtAddress)
{
	size_t pageNumber = virtAddress / PAGE_SIZE,
	offset = virtAddress % PAGE_SIZE;

	//if page is in memory, translate to physical address
	if(pcb->is_valid_page(pageNumber))
	{
		return pcb->get_frame(pageNumber) * PAGE_SIZE + offset;
	}

	else
	{
		//page fault, store requested page number and wait for the fault to be serviced
		pcb->set_waitformmu(true);
		pcb->page_request = pageNumber;
		return 0xDEADBEEF;

		//we have no free frames, so we must replace one
		/*
		else
		{
			pageReplace = pcb->pop_lru_page();
			size_t victimFrame = pcb->get_frame(pageReplace);

			writePageToDisk(pcb, pageReplace);
			pcb->set_page_table_entry(pageReplace, false, -1);

			readPageFromDisk(pcb, pageNumber, victimFrame);
			pcb->set_page_table_entry(pageNumber, true, victimFrame);
			pcb->update_page_stack(pageNumber);
			return victimFrame;
		}
		*/
	}
}
//MODIFICATION
void mmu::allocateFrame(PCB* pcb)
{
	if(!_freeFrames.empty() && pcb->page_request != 0xDEADBEEF)
	{
		size_t frame = _freeFrames.back();
		_freeFrames.pop();

		readPageFromDisk(pcb, pcb->page_request, frame);
		pcb->set_page_table_entry(pcb->page_request, true, frame);
		pcb->set_waitformmu(false);
	}
}

void mmu::tableInit(PCB* pcb, size_t frameCount)
{
	size_t frame;
	if(_freeFrames.size() >= frameCount)
	{
		for(unsigned int i = 0; i < frameCount; i++)
		{
			frame = _freeFrames.back();
			_freeFrames.pop();

			pcb->set_page_table_entry(i, true, frame);
			readPageFromDisk(pcb,(size_t)(pcb->get_disk_address() / PAGE_SIZE),frame);
		}
	}
}
void mmu::dumpProcess(PCB* pcb)
{
	size_t frame;
	for(unsigned int i = 0; i < pcb->get_page_table_length(); i++)
	{
		if(pcb->is_valid_page(i))
		{
			frame = pcb->get_frame(i);
			writePageToDisk(pcb, i);
			pcb->set_page_table_entry(i, false, -1);
			_freeFrames.push(frame);
		}
	}
}

//SWAPPING
void mmu::readPageFromDisk(PCB* pcb, size_t pageNumber, size_t frameNumber)
{
	// So we don't get error for unused variable.
	size_t diskLoc = frameNumber;
	size_t ramLoc;
	for(int i = 0; i < PAGE_SIZE; i++)
	{
		diskLoc = pcb->get_ram_address() + pageNumber * PAGE_SIZE + i;
		ramLoc = frameNumber * PAGE_SIZE + i;

		//Make sure process doesn't read beyond its size
		if(diskLoc - pcb->get_ram_address() >= pcb->get_end_address())
			break;

		MEM.allocate(ramLoc, DISK.read_byte(diskLoc));
	}
}
void mmu::writePageToDisk(PCB* pcb, size_t pageNumber)
{
	size_t frameNumber = pcb->get_frame(pageNumber);
	size_t ramLoc;
	// So we don't get compiler errors from unused variable.
	size_t diskLoc = frameNumber;
	for(int i = 0; i < PAGE_SIZE; i++)
	{
		diskLoc = pcb->get_ram_address() + pageNumber * PAGE_SIZE + i;
		ramLoc = frameNumber * PAGE_SIZE + i;

		//Make sure process doesn't write over another process
		if(diskLoc - pcb->get_ram_address() >= pcb->get_end_address())
			break;

		DISK.allocate(diskLoc, DISK.read_byte(ramLoc));
	}
}

//GLOBAL VARIABLE
mmu MMU;


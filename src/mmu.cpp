#include "mmu.hpp"
#include "ram.hpp"

MMU::MMU()
{
	for(int i = 0; i < MEM.size() / PAGE_SIZE; i++)
		_freeFrames.push(i);
}

std::size_t MMU::getPhysicalAddress(PCB pcb, std::size_t virtAddress)
{
	std::size_t pageNumber = virtAddress / PAGE_SIZE, 
	offset = virtAddress % PAGE_SIZE;
	
	//if page is in memory, translate to physical and update page stack
	if(pcb.is_valid(pageNumber))
	{	
		pcb.update_page_stack(pageNumber);
		return pcb.get_frame(pageNumber) * PAGE_SIZE + offset;
	}
	
	else
	{
		//we have free frames, so we give one to the process and update page 
		//table and stack
		if(!_freeFrames.empty())
		{
			std::size_t frame = _freeFrames.pop();
			
			readPageFromDisk(pcb, pageNumber, frame);
			pcb.set_page_table_entry(pageNumber, true, frame);
			pcb.update_page_stack(pageNumber);
		}
		
		//we have no free frames, so we must replace one 
		else
		{
			std::size_t pageReplace = pcb.pop_lru_page();
			std::size_t victimFrame = pcb.get_frame(pageReplace);
			
			writePageToDisk(pcb, pageReplace);
			pcb.set_page_table_entry(pageReplace, false, -1);
			
			readPageFromDisk(pcb, pageNumber, victimFrame);
			pcb.set_page_table_entry(pageNumber, true, victimFrame);
			pcb.update_page_stack(pageNumber);
		}
	}
}

//MODIFICATION
void MMU::tableInit(PCB pcb, std::size_t frameCount)
{
	//If there are at least 3 free frames, allocate them to this process
	if(_freeFrames.size() >= frameCount)
	{
		for(int i = 0; i < frameCount; i++)
		{
			std::size_t frame = _freeFrames.pop();
			
			pcb.set_page_table_entry(i, true, frame);
			readPageFromDisk(pcb.get_disk_address(), pcb.get_end_address(), i, frame); 
		}
	}
}

void MMU::dumpProcess(PCB pcb)
{
	for(int i = 0; i < pcb.get_page_table_length(); i++)
	{
		if(pcb.is_valid_page(i))
		{
			std::size_t frame = pcb.get_frame(i);
			
			writePageToDisk(pcb, i);
			pcb.set_page_table_entry(pageReplace, false, -1);
			
			_freeFrames.push(frame);
		}
	}
}

//SWAPPING
void MMU::readPageFromDisk(PCB pcb, std::size_t pageNumber, std::size_t frameNumber)
{
	for(int i = 0; i < PAGE_SIZE; i++)
	{
		std::size_t diskLoc = pcb.get_start_address() + pageNumber * PAGE_SIZE + i,
					ramLoc = frameNumber * FRAME_SIZE + i;
		
		//Make sure process doesn't read beyond its size
		if(diskLoc - pcb.get_start_address() >= pcb.get_end_address())
			break;
		
		RAM.allocate(ramLoc, DISK.read_byte(diskLoc));
	}
}

void MMU::writePageToDisk(PCB pcb, std::size_t pageNumber)
{
	std::size_t frameNumber = pcb.get_frame(pageNumber);
	
	for(int i = 0; i < PAGE_SIZE; i++)
	{
		std::size_t diskLoc = pcb.get_start_address() + pageNumber * PAGE_SIZE + i,
					ramLoc = frameNumber * FRAME_SIZE + i;
		
		//Make sure process doesn't write over another process
		if(diskLoc - pcb.get_start_address() >= pcb.get_end_address())
			break;
		
		DISK.allocate(diskLoc, RAM.read_byte(ramLoc));
	}
}
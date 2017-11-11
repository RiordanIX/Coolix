#include "mmu.hpp"
#include "ram.hpp"

MMU::MMU()
{
	for(int i = 0; i < MEM.size() / PAGE_SIZE; i++)
		_freeFrames.push(i);
}

//PAGING AND PAGE TABLE
void MMU::tableInit(PCB pcb, std::size_t diskAddress)
{
	//If there are at least 3 free frames, allocate them to this process
	if(_freeFrames.size() >= 3)
	{
		for(int i = 0; i < 3; i++)
		{
			std::size_t frame = _freeFrames.pop();
			
			pcb.set_page_table_entry(i, true, frame);
			readPageFromDisk(pcb.get_disk_address(), pcb.get_end_address(), i, frame); 
		}
	}
}

std::size_t MMU::getPhysicalAddress(PCB pcb, std::size_t virtAddress)
{
	std::size_t pageNumber = virtAddress / PAGE_SIZE,
				offset = virtAddress % PAGE_SIZE,
				frame = pcb.get_frame(pageNumber);
	
	if(frame != -1)
	{
		return frame * PAGE_SIZE + offset;
	}
}

//SWAPPING
void MMU::readPageFromDisk(std::size_t startAddress, std::size_t endAddress, std::size_t pageNumber, std::size_t frameNumber)
{
	for(int i = 0; i < PAGE_SIZE; i++)
	{
		std::size_t diskLoc = startAddress + pageNumber * PAGE_SIZE + i,
					ramLoc = frameNumber * FRAME_SIZE + i;
		
		//Make sure process doesn't read beyond its size
		if(diskLoc - startAddress >= endAddress)
			break;
		
		RAM.allocate(ramLoc, DISK.read_byte(diskLoc));
	}
}

void MMU::writePageToDisk(std::size_t startAddress, std::size_t endAddress, std::size_t pageNumber, std::size_t frameNumber)
{
	for(int i = 0; i < PAGE_SIZE; i++)
	{
		std::size_t diskLoc = startAddress + pageNumber * PAGE_SIZE + i,
					ramLoc = frameNumber * FRAME_SIZE + i;
		
		//Make sure process doesn't write over another process
		if(diskLoc - startAddress >= endAddress)
			break;
		
		DISK.alllocate(diskLoc, RAM.read_byte(ramLoc));
	}
}
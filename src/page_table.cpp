#include "page_table.hpp"

public PageTable::PageTable(Ram ram, PageDirectory dir) : _frameLocations(3, -1);
{
	_ram = ram;
	
	int setFrames = 0
	
	//Find available frames for the page table and stop when the table has three associated frames.
	for(int i = 0; i < dir.frames.size(); i++)
	{
		if(dir.frames[i] == 0)
		{
			_frameLocations.at(setFrames) = i;
			setFrames++;
			
			if(setFrames >= 3)
				break;
		}
	}
}

public byte_t PageTable::readPhysicalAddress(unsigned int page, unsigned int offset)
{
	return _ram.get_instruction(_frameLocations[page] * FRAME_SIZE + offset) 
}
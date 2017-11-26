#include "mmu.hpp"

extern Ram MEM;
extern Disk DISK;

using std::size_t;

mmu::mmu() {

	SetFreeFrames();
}
void mmu::SetFreeFrames()
{
	for (unsigned int i = 0; i < (MEM.size()/(PAGE_SIZE)); i++)
		addFreeFrame(i);
	debug_printf("Available Frames:\t%lu\tMEM size\t%lu\tPage size\t%d\n", free_frame_count(), MEM.size(), (PAGE_SIZE));
//	for (auto i = _freeFrames.begin(); i != _freeFrames.end(); ++i)
//		debug_printf("%lu ", *i);
}
size_t mmu::getPhysicalAddress(PCB* pcb, size_t virtAddress) {
	size_t pageNumber = virtAddress / (PAGE_SIZE),
	offset = virtAddress % (PAGE_SIZE);

	//if page is in memory, translate to physical and update page stack
	if(pcb->is_valid_page(pageNumber))
	{
		pcb->update_page_stack(pageNumber);
		return pcb->get_frame(pageNumber) * (PAGE_SIZE) + offset;
	}

	else
	{
		//we have free frames, so we give one to the process and update page
		//table and stack
		if(!_freeFrames.empty())
		{
			size_t frame = _freeFrames.front();
			_freeFrames.pop();

			processDiskToRam(pcb, pageNumber);
			pcb->set_page_table_entry(pageNumber, true, frame);
			pcb->update_page_stack(pageNumber);
			return frame;
		}

		else
		{
			pcb->set_waitformmu(true);
			ShortTermScheduler::RunningToWait(pcb);
			return -1;
		}
		/*
		//we have no free frames, so we must replace one
		else
		{
			pageReplace = pcb->pop_lru_page();
			size_t victimFrame = pcb->get_frame(pageReplace);

			writePageToDisk(pcb, pageReplace);
			pcb->set_page_table_entry(pageReplace, false, -1);

			processDiskToRam(pcb, pageNumber);
			pcb->set_page_table_entry(pageNumber, true, victimFrame);
			pcb->update_page_stack(pageNumber);
			return victimFrame;
		}
		*/
	}
}



//MODIFICATION
void mmu::tableInit(PCB* pcb, size_t frameCount)
{
	size_t frame;
	if(_freeFrames.size() >= frameCount)
	{
		for(unsigned int i = 0; i < frameCount; i++)
		{
			frame = _freeFrames.front();
			_freeFrames.pop();

			pcb->set_page_table_entry(i, true, frame);
			processDiskToRam(pcb,(size_t)(pcb->get_disk_address() / (PAGE_SIZE)));
		}
	}
}



void mmu::dumpProcess(PCB* pcb) {
	size_t frame;
	for(unsigned int i = 0; i < pcb->get_page_table_length(); i++)
	{
		if(pcb->is_valid_page(i))
		{
			frame = pcb->get_frame(i);
			writePageToDisk(pcb, i);
			pcb->set_page_table_entry(pageReplace, false, -1);
			_freeFrames.push(frame);
		}
	}
}



std::size_t mmu::FrameNumberToLocation(size_t Frame)
{
	return Frame * PAGE_SIZE;
}



//SWAPPING
bool mmu::processDiskToRam(PCB* pcb, size_t pageNumber) {
	size_t diskLoc;
	size_t frameNum;
	size_t address;
	// No frames are available, failed
	if (free_frame_count()== 0)
	{
		return false;
	}
	diskLoc = pcb->get_disk_address() + pageNumber * (PAGE_SIZE);
	frameNum = _freeFrames.front();
	address = FrameNumberToLocation(frameNum);


	// Sanity check to make sure not allocating more RAM than we have.
	if (address + (PAGE_SIZE) > MEM.size())
	{
		return false;
	}

	// We're confident the frame can be popped.
	_freeFrames.pop();

	// Now actually allocate data to main memory
	for(int i = 0; i < ((PAGE_SIZE)/ (INST_SIZE)); i++, diskLoc+=4, address+=4) {
		try {
			MEM.allocate(address, DISK.read_instruction((diskLoc)));
			pcb->set_page_table_entry(i, true, frameNum);
		}
		catch (char* e) {
			printf("%s",e);
			return false;
		}
	}
	return true;
}


void mmu::writeToRam(instruct_t location, instruct_t data) {

}




void mmu::writePageToDisk(PCB* pcb, size_t pageNumber) {
	size_t frameNumber = pcb->get_frame(pageNumber);
	//size_t ramLoc;
	// So we don't get compiler errors from unused variable.
	size_t diskLoc = frameNumber;
	for(int i = 0; i < (PAGE_SIZE); i++)
	{
		 diskLoc = pcb->get_ram_address() + pageNumber * (PAGE_SIZE) + i;
		 //ramLoc = frameNumber * PAGE_SIZE + i;

		//Make sure process doesn't write over another process
		if(diskLoc - pcb->get_ram_address() >= pcb->get_end_address())
			break;

		//DISK.allocate(diskLoc, DISK.read_byte(ramLoc));
	}
}



instruct_t mmu::get_instruction(PCB* pcb) 
{
	size_t frame;
	size_t address;
	size_t offset;
	if (pcb->is_valid_page(pcb->get_program_counter()/(PAGE_SIZE))){
		frame = pcb->get_frame(pcb->get_program_counter() / (PAGE_SIZE));
	}
	else {
	}
	offset = (pcb->get_program_counter()) % (INST_SIZE);
	address = MEM.get_instruction(frame*(PAGE_SIZE) + offset);
	return address;
}


instruct_t mmu::get_instruction(PCB* pcb, instruct_t address)
{
	size_t frame;
	if (pcb->is_valid_page(address / (PAGE_SIZE)))
	{
		frame = pcb->get_frame(address / (PAGE_SIZE));
	}
	else;
	return address;
}
// GLOBAL VARIABLE
mmu MMU;


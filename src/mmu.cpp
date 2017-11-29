#include "mmu.hpp"
#include <mutex>

extern Ram MEM;
extern Disk DISK;
extern int mmuLock;
extern int FrameDump;
using std::size_t;
using std::vector;

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
			pcb->set_page_table_entry(i, false, -1);
			while (FrameDump == 1) { printf("framelock"); }
			FrameDump = 1;
			_freeFrames.push(frame);
			FrameDump = 0;
		}
	}
}
void mmu::dumpPage(PCB* pcb) {
	size_t frame;
	for (unsigned int i = (pcb->get_page_table_length() - 1); i > 0; i--)
	{
		if (pcb->is_valid_page(i))
		{
			frame = pcb->get_frame(i);
			writePageToDisk(pcb, i);
			pcb->set_page_table_entry(i, false, -1);
			while (FrameDump == 1) { printf("framelock"); }
			FrameDump = 1;
			_freeFrames.push(frame);
			FrameDump = 0;
			break;
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
	size_t fsize = 0;
	while (FrameDump == 1) { printf("framelock"); }
	FrameDump = 1;
	fsize = _freeFrames.size();
	FrameDump = 0;
	// No frames are available, failed
	
	if (fsize == 0)
	{
		return false;
	}
	
	diskLoc =  pcb->get_disk_address() + ((pageNumber)*(PAGE_SIZE));
	while (FrameDump == 1) { printf("framelock"); }
	FrameDump = 1;
	frameNum = _freeFrames.front();
	FrameDump = 0;
	
	address = FrameNumberToLocation(frameNum);
	

	// Sanity check to make sure not allocating more RAM than we have.
	if (address + (PAGE_SIZE) > MEM.size())
	{
		return false;
	}

	// We're confident the frame can be popped.
	while (FrameDump == 1) { printf("framelock"); }
	FrameDump = 1;
	_freeFrames.pop();
	FrameDump = 0;
	pcb->set_page_table_entry(pageNumber, true, frameNum);
	// Now actually allocate data to main memory
	while (mmuLock == 1) { printf("mmu6"); }
	mmuLock = 1;
	for(int i = 0; i < ((PAGE_SIZE)/ (INST_SIZE)); i++, diskLoc+=4, address+=4) {
		try {
			MEM.allocate(address, DISK.read_instruction((diskLoc)));
		}
		catch (char* e) {
			printf("%s",e);
			mmuLock = 0;
			return false;
		}
	}
	mmuLock = 0;
	return true;
}


void mmu::writeToRam(instruct_t frame,instruct_t offset, instruct_t data) {
	// Need to make sure that the frame is active. Otherwise, wrong data
	instruct_t address = FrameNumberToLocation(frame);
	while (mmuLock == 1) { printf("mmu5"); }
	mmuLock = 1;
	MEM.allocate(address + offset, data);
	mmuLock = 0;
}




/*void mmu::writePageToDisk(PCB* pcb, size_t pageNumber) {
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
}*/
void mmu::writePageToDisk(PCB* pcb, size_t pageNumber)
{
	size_t frameNumber = pcb->get_frame(pageNumber);
	size_t address = FrameNumberToLocation(frameNumber);
	//size_t ramLoc;
	// So we don't get compiler errors from unused variable.
	size_t diskLoc = pcb->get_disk_address() + (pageNumber * (PAGE_SIZE));
	while (mmuLock == 1) { printf("mmu4"); }
	mmuLock = 1;
	for (int i = 0; i < ((PAGE_SIZE) / (INST_SIZE)); i++, diskLoc += 4, address += 4)
	{
		DISK.allocate(diskLoc, MEM.get_instruction(address));
	}
	mmuLock = 0;
}

instruct_t mmu::get_instruction(PCB* pcb)
{
	size_t frame;
	size_t offset;
	pcb->set_lastRequestedPage(pcb->get_program_counter() / (PAGE_SIZE));
	if (pcb->is_valid_page(pcb->get_program_counter()/(PAGE_SIZE))){
		frame = pcb->get_frame(pcb->get_program_counter() / (PAGE_SIZE));
		offset = (pcb->get_program_counter() % (PAGE_SIZE));
		while (mmuLock == 1) { printf("mmu3"); }
		mmuLock = 1;
		instruct_t results = MEM.get_instruction((frame * (PAGE_SIZE)) + offset);
		mmuLock = 0;
		return results;
	}
	else {
		// Page Fault
		return -1;
	}
}

vector<instruct_t> mmu::get_frame_data(PCB* pcb) {
	unsigned int counter = pcb->get_program_counter();
	size_t offset = (pcb->get_program_counter() % (PAGE_SIZE));
	vector<instruct_t> insts;
	insts.resize(8);
	for (unsigned int i = 0; i < 4; i++, counter +=4) {
		offset = (offset + counter) / (4);
		insts[offset]=(get_instructionCache(pcb,counter));
	}
	return insts;
}
instruct_t mmu::get_instructionCache(PCB* pcb, instruct_t address)
{
	size_t pageNumber = (address / (PAGE_SIZE));
	pcb->set_lastRequestedPage(pageNumber);
	if (pcb->is_valid_page(pageNumber))
	{
		size_t frame = pcb->get_frame(pageNumber);
		size_t offset = (address % (PAGE_SIZE));
		while (mmuLock == 1) { printf("mmu2"); }
		mmuLock = 1;
		instruct_t results = MEM.get_instruction((frame * (PAGE_SIZE)) + offset);
		mmuLock = 0;
		return results;
	}
	else {
		//page fault
		return -1;
	}
}

instruct_t mmu::get_instruction(PCB* pcb, instruct_t address)
{
	size_t pageNumber = (address / (PAGE_SIZE));
	if (pageNumber > 100)
	{
		int value = 0;
	}
	pcb->set_lastRequestedPage(pageNumber);
		if (pcb->is_valid_page(pageNumber))
		{
			size_t frame = pcb->get_frame(pageNumber);
			printf("mmu1");
			while (mmuLock == 1)
			{
				printf("mmu1");
			}
			mmuLock = 1;
			instruct_t results = MEM.get_instruction((frame* (PAGE_SIZE)) + (address % (PAGE_SIZE)));
			mmuLock = 0;
			return results;
		}
		else {
			//page fault
			return -1;
		}
}


instruct_t mmu::getRamAddress(PCB * pcb, instruct_t localAddress)
{
	size_t frame = pcb->get_frame(pcb->get_program_counter() / (PAGE_SIZE));
	pcb->set_lastRequestedPage(pcb->get_program_counter() / (PAGE_SIZE));
	if (pcb->is_valid_page(pcb->get_program_counter() / (PAGE_SIZE))) {

		return (frame* (PAGE_SIZE) + (localAddress % (PAGE_SIZE)));
	}
	else {
		// Page Fault
		return -1;
	}
}



// GLOBAL VARIABLE
mmu MMU;


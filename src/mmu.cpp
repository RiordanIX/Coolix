#include "mmu.hpp"
#include <mutex>

extern Ram MEM;
extern Disk DISK;
using std::size_t;
using std::vector;

mmu::mmu() {

	SetFreeFrames();
}
//load free frames in free frame queue
void mmu::SetFreeFrames()
{
	for (unsigned int i = 0; i < (MEM.size()/(PAGE_SIZE)); i++)
		addFreeFrame(i);
	debug_printf("Available Frames:\t%lu\tMEM size\t%lu\tPage size\t%d\n", free_frame_count(), MEM.size(), (PAGE_SIZE));
//	for (auto i = _freeFrames.begin(); i != _freeFrames.end(); ++i)
//		debug_printf("%lu ", *i);
}
//free frame from process
void mmu::dumpProcess(PCB* pcb) {
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
//free only one from process
void mmu::dumpPage(PCB* pcb) {
	size_t frame;
	for (unsigned int i = (pcb->get_page_table_length() - 1); i > 0; i--)
	{
		if (pcb->is_valid_page(i))
		{
			frame = pcb->get_frame(i);
			writePageToDisk(pcb, i);
			pcb->set_page_table_entry(i, false, -1);
			_freeFrames.push(frame);
			break;
		}
	}
}


std::size_t mmu::FrameNumberToLocation(size_t Frame)
{
	return Frame * PAGE_SIZE;
}

//Move process from disk to ram
bool mmu::processDiskToRam(PCB* pcb, size_t pageNumber) {
	
	size_t diskLoc;
	size_t frameNum;
	size_t address;
	size_t fsize = 0;
	fsize = _freeFrames.size();
	// No frames are available, failed
	if (fsize == 0)
	{
		return false;
	}
	
	diskLoc =  pcb->get_disk_address() + ((pageNumber)*(PAGE_SIZE));
	frameNum = _freeFrames.front();
	address = FrameNumberToLocation(frameNum);
	

	// Sanity check to make sure not allocating more RAM than we have.
	if (address + (PAGE_SIZE) > MEM.size())
	{
		return false;
	}

	// We're confident the frame can be popped.
	_freeFrames.pop();
	pcb->set_page_table_entry(pageNumber, true, frameNum);
	// Now actually allocate data to main memory
	setLock();
	for(int i = 0; i < ((PAGE_SIZE)/ (INST_SIZE)); i++, diskLoc+=4, address+=4) {
		try {
			MEM.allocate(address, DISK.read_instruction((diskLoc)));
		}
		catch (char* e) {
			printf("%s",e);
			freeLock();
			return false;
		}
	}
	freeLock();
	return true;
}

void mmu::writeToRam(instruct_t frame,instruct_t offset, instruct_t data) {
	// Need to make sure that the frame is active. Otherwise, wrong data
	instruct_t address = FrameNumberToLocation(frame);
	setLock();
	MEM.allocate(address + offset, data);
	freeLock();
}
void mmu::writePageToDisk(PCB* pcb, size_t pageNumber)
{
	size_t frameNumber = pcb->get_frame(pageNumber);
	size_t address = FrameNumberToLocation(frameNumber);
	//size_t ramLoc;
	// So we don't get compiler errors from unused variable.
	size_t diskLoc = pcb->get_disk_address() + (pageNumber * (PAGE_SIZE));
	setLock();
	for (int i = 0; i < ((PAGE_SIZE) / (INST_SIZE)); i++, diskLoc += 4, address += 4)
	{
		DISK.allocate(diskLoc, MEM.get_instruction(address));
	}
	freeLock();
}

instruct_t mmu::get_instruction(PCB* pcb)
{
	size_t frame;
	size_t offset;
	pcb->set_lastRequestedPage(pcb->get_program_counter() / (PAGE_SIZE));
	if (pcb->is_valid_page(pcb->get_program_counter()/(PAGE_SIZE))){
		frame = pcb->get_frame(pcb->get_program_counter() / (PAGE_SIZE));
		offset = (pcb->get_program_counter() % (PAGE_SIZE));
		setLock();
		instruct_t results = MEM.get_instruction((frame * (PAGE_SIZE)) + offset);
		freeLock();
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
	insts.resize(4); //size of 4 instructions in 1 frame
	for (unsigned int i = 0; i < 4; i++, counter +=4) {
		offset = (counter % (PAGE_SIZE)) / (4);
		insts[offset]=(get_instruction(pcb,counter));
	}
	return insts;
}
instruct_t mmu::get_instruction(PCB* pcb, instruct_t address)
{
	size_t pageNumber = (address / (PAGE_SIZE));
	pcb->set_lastRequestedPage(pageNumber);
		if (pcb->is_valid_page(pageNumber))
		{
			size_t frame = pcb->get_frame(pageNumber);
			printf("mmu1");
			setLock();
			instruct_t results = MEM.get_instruction((frame* (PAGE_SIZE)) + (address % (PAGE_SIZE)));
			freeLock();
			return results;
		}
		else {
			//page fault
			return -1;
		}
}

// GLOBAL VARIABLE
mmu MMU;


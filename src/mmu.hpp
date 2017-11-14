#include <queue>
#include "ram.hpp"
#include "pcb.hpp"

#define PAGE_SIZE 128

class MMU
{
	public:
		MMU();
		
		//GETTERS
		std::size_t free_frame_count()	{ return _freeFrames.size(); }
		std::size_t getPhysicalAddress(PageTable table, std::size_t virtAddress);
		
		//MODIFICATION
		void tableInit(PageTable table);
		void dumpProcess(PCB pcb);
	
	private:
		std::queue<std::size_t> _freeFrames;
		
		//SWAPPING
		void readPageFromDisk(std::size_t startAddress, std::size_t endAddress, std::size_t pageNumber, std::size_t frameNumber);
		void writePageToDisk(std::size_t startAddress, std::size_t endAddress, std::size_t pageNumber, std::size_t frameNumber);
		
}
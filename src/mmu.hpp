#include <queue>
#include "ram.hpp"
#include "pcb.hpp"

#define PAGE_SIZE 128

class MMU
{
	public:
		MMU();
		
		//PAGING
		std::size_t getPhysicalAddress(PageTable table, std::size_t virtAddress);
		void tableInit(PageTable table);
	
	private:
		std::vector<std::size_t> _diskPages;
		std::queue<std::size_t> _freeFrames;
		
		//SWAPPING
		void readPageFromDisk(std::size_t startAddress, std::size_t endAddress, std::size_t pageNumber, std::size_t frameNumber);
		void writePageToDisk(std::size_t startAddress, std::size_t endAddress, std::size_t pageNumber, std::size_t frameNumber);
		
}
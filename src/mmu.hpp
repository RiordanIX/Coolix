#include "ram.hpp"
#include "pcb.hpp"
#include "disk.hpp"
#include "ShortTerm.hpp"
#include <queue>

class MMU
{
	public:
		MMU();
		
		//GETTERS
		std::size_t free_frame_count()	{ return _freeFrames.size(); }
		std::size_t getPhysicalAddress(PCB* pcb);
		
		//MODIFICATION
		void allocateFrame(PCB* pcb);
		void tableInit(PCB* pcb, std::size_t frameCount);
		void dumpProcess(PCB* pcb);
	
	private:
		std::queue<std::size_t> _freeFrames;
		std::size_t pageReplace;
		//SWAPPING
		void readPageFromDisk(PCB* pcb, std::size_t pageNumber, std::size_t frameNumber);
		void writePageToDisk(PCB* pcb, std::size_t pageNumber);
		
};
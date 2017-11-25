#include "ram.hpp"
#include "pcb.hpp"
#include "disk.hpp"
#include "ShortTerm.hpp"
#include <queue>

class mmu
{
	public:
		mmu();

		//GETTERS
		std::size_t free_frame_count()	{ return _freeFrames.size(); }
		std::size_t getPhysicalAddress(PCB* pcb, std::size_t virtAddress);

		//MODIFICATION
		void tableInit(PCB* pcb, std::size_t frameCount);
		void dumpProcess(PCB* pcb);
		bool processDiskToRam(PCB* pcb, std::size_t pageNumber);

	private:
		std::queue<std::size_t> _freeFrames;
		std::size_t pageReplace;
		//SWAPPING
		void writePageToDisk(PCB* pcb, std::size_t pageNumber);

};


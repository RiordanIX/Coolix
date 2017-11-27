#include "instruct.hpp"
#include "ram.hpp"
#include "pcb.hpp"
#include "disk.hpp"
#include "cache.hpp"
#include <queue>
#include <vector>

class mmu
{
	public:
		mmu();

		void SetFreeFrames();

		//GETTERS
		std::size_t free_frame_count()	{ return _freeFrames.size(); }

		//MODIFICATION
		void addFreeFrame(int x) { _freeFrames.push(x); };
		void tableInit(PCB* pcb, std::size_t frameCount);
		void dumpProcess(PCB* pcb);
		size_t FrameNumberToLocation(std::size_t Frame);
		bool processDiskToRam(PCB* pcb, std::size_t pageNumber);
		void writeToRam(instruct_t location, instruct_t offset, instruct_t data);
		instruct_t get_instruction(PCB* pcb);

		instruct_t getRamAddress(PCB * pcb, instruct_t offset);

		instruct_t get_instruction(PCB* pcb, instruct_t address);
		std::vector<instruct_t> get_frame_data(PCB* pcb);

		instruct_t get_instructionCache(PCB * pcb, instruct_t address);

	private:
		std::queue<std::size_t> _freeFrames;
		std::size_t pageReplace;
		//SWAPPING
		void writePageToDisk(PCB* pcb, std::size_t pageNumber);
};


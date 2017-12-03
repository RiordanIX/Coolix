#pragma once
#include <algorithm>
#include <vector>
#include "mmu.hpp"
#include "disk.hpp"
#include "ram.hpp"
#include "pcb.hpp"
#include "pcbQueue.hpp"
//#include "FIFO.hpp"


class LongTerm
{
public:
	std::size_t FrameSize();
	//distructor
	//Move  processes from disk to ram
	static void loadProcess(PCB * pcb,std::size_t pagenumber);
	bool loadPage(PCB * pcb, std::size_t pagenumber);
	static void DumpProcess(PCB * pcb);
	static void DumpFrame(PCB * pcb);
	static int initialLoad();
};


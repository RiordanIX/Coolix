#pragma once
#include <algorithm>
#include <vector>
#include "mmu.hpp"
#include "disk.hpp"
#include "ram.hpp"
#include "pcb.hpp"
#include "PriorityQueue.hpp"
//#include "FIFO.hpp"


class LongTerm
{
public:
	LongTerm();//contructor
	~LongTerm();//distructor
	//Move  processes from disk to ram
	void loadProcess(PCB * pcb,std::size_t pagenumber);
	void loadPage(PCB * pcb, std::size_t pagenumber);
	void DumpProcess(PCB * pcb);
private:
	struct EmptySpace
	{
		size_t Sadd;//start address
		size_t Isize;//instruction size
		EmptySpace(size_t sadd, size_t isize)
			:Sadd(sadd),Isize(isize){ }
	};
	struct Used
	{
		size_t Start;//start address
		size_t End;//End address
		Used(size_t st, size_t ed): Start(st), End(ed){ }
	};
	struct SortUsedSpace {
		bool operator() (Used i, Used j) { return ((i.Start+i.End)<(j.Start+i.End)); }
	} SortUsed;
	std::vector<EmptySpace> GetOpenSpaces();
	bool CheckEmpty(EmptySpace es, std::vector<Used> used);
	bool CheckResource(resourceType RT);
	size_t ReadySize;
	instruct_t MaxAddress;
	std::vector<Used> used;
};


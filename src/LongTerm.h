#pragma once
#include "disk.hpp"
#include "ram.hpp"
#include <vector>
#include "pcb.hpp"
#include "PriorityQueue.h"


class LongTerm
{
public:
	LongTerm();//contructor 
	~LongTerm();//distructor
	//Move  processes from disk to ram
	void DiskToRam();
	//move io bond process from ready to wait queue if io is busy
	void ReadyToWait();
	//move io bond process from ready to wait queue if io is busy
	void WaitToReady();
private:
	struct EmptySpace
	{
		int Sadd;//start address
		int Isize;//instruction size
		EmptySpace(int sadd, int isize){
			Sadd = sadd;
			Isize = isize;
		}
	};
	struct Used
	{
		int Start;//start address
		int End;//End address
		Used(int st, int ed) {
			Start = st;
			End = ed;
		}
	};
	std::vector<EmptySpace> GetOpenSpaces();
	bool CheckEmpty(EmptySpace es, std::vector<Used> used);
	size_t ReadySize;
	instruct_t MaxAddress;
};
extern std::vector<PCB> process_list;


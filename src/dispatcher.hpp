#pragma once
#include <deque>
#include "debug.hpp"
#include "instruct.hpp"
#include "pcb.hpp"
#include "PriorityQueue.hpp"
#include "cpu.hpp"

//  TODO: THINGS THAT I DID NOT IMPLEMENT YET
//  - Counting CPU Cycle
//  - Record Keeping


class Dispatcher {
public:
	// Context Switches. Current process goes out AND new process goes in.
	// CPU = which CPU. cProcess = Current Process.
	void dispatch(cpu* CPU, PCB* cProcess);
	static void switchOut(cpu* CPU, PCB* cProcess);

private:
	//  Copies CPU Register Values to PCB Register Values. Record Keeping
	//  Variables. Move Process to Waiting Queue
	
	void switchIn(cpu * CPU, PCB * cProcess);
	//  Copies PCB Register Values to CPU Register Values. Record Keeping
	//  Variables. Move Process into CPU.
	//void switchIn(cpu* CPU);
};


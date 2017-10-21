#pragma once
#include <deque>
#include "instruct.hpp"
#include "pcb.hpp"
#include "PriorityQueue.h"
#include "cpu.hpp"

//  THINGS THAT I DID NOT IMPLEMENT YET
//  - Counting CPU Cycle
//  - Record Keeping

extern PriorityQueue readyQueue;

class Dispatcher
{
public:
    void dispatch(cpu* CPU, PCB* cProcess, int cCycle, int maxCycle);    //  Context Switches. Current process goes out AND new process goes in                                                              // CPU = Which CPU.   cProcess = Current Process.     nProcess = The Next Process

private:
    void switchOut(cpu* CPU, PCB* cProcess);    //  Copies CPU Register Values to PCB Register Values. Record Keeping Variables. Move Process to Waiting Queue
    void switchIn(cpu* CPU);     //  Copies PCB Register Values to CPU Register Values. Record Keeping Variables. Move Process into CPU.

};


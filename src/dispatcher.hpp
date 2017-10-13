#pragma once
#include <deque>
#include "instruct.hpp"
#include "pcb.hpp"

//  THINGS THAT I DID NOT IMPLEMENT YET
//  - Counting CPU Cycle
//  - Record Keeping

class Dispatcher
{
public: 
    int count_cpu_cycle = 0;
    void dispatch(cpu* CPU, PCB* cProcess, PCB* nProcess);    //  Context Switches. Current process goes out AND new process goes in                                                              // CPU = Which CPU.   cProcess = Current Process.     nProcess = The Next Process

private:
    void switchOut(cpu* CPU, PCB* cProcess);    //  Copies CPU Register Values to PCB Register Values. Record Keeping Variables. Move Process to Waiting Queue
    void switchIn(cpu* CPU, PCB* nProcess);     //  Copies PCB Register Values to CPU Register Values. Record Keeping Variables. Move Process into CPU.
    
}
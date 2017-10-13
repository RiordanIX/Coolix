#include "dispatcher.hpp"
#include "PriorityQueue.h"
#include "std.h"

void Dispatcher::dispatch(cpu* CPU, PCB* cProcess, PCB* nProcess, bool wait)
{
    count_cpu_cycle++;
    if(!wait)
    {
        switchOut(CPU, cProcess);
        switchIn(CPU, nProcess);
    }
}

void Dispatcher::switchIn(cpu* CPU)     //  switchOut should occur first, therefore..
{
    for(int i = 0; i < CPU.registers.size(); i++)
    {
        readyQueue.Q.pop();                                         // Removes the Current Running Process  
        CPU.registers[i] = readyQueue.getProcess().registers[i];    // Set the CPU Registers to the Registers of the next Process in the readyQueue            
        //CPU.CurrentProcess = readyQueue.getProcess();             //  Assigns the new First Process in the Ready Queue to the current Process in the CPU
    }
}

void Dispatcher::switchOut(cpu* CPU, PCB* cProcess)
{
     for(int i = 0; i < CPU.registers.size(); i++)
    {
        cProcess.registers[i] = CPU.registers[i];
        PCB* exitingProcess = readyQueue.getProcess();  //  Creates a pointer to the Current Running Process, calling it "exitingProcess"
        waitingQueue.Q.push(exitingProcess);            //  Moves the exiting process into the Waiting Queue
    }
}


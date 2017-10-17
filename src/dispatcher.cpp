#include "dispatcher.hpp"
#include "PriorityQueue.h"
#include "std.h"

void Dispatcher::dispatch(cpu* CPU, PCB* cProcess, bool wait)
{
    count_cpu_cycle+;
    if(wait)
    {
        switchOut(CPU, cProcess);   //  Moves current process to WaitingQueue
        switchIn(CPU);              //  Removes First Process and gives next process to CPU
    }
}

void Dispatcher::switchIn(cpu* CPU)
{
    for(int i = 0; i < CPU.registers.size(); i++)
    {
        readyQueue.Q.pop();                                         //  Removes the Current Running Process, so the next one will be the new active process                        
        CPU.registers[i] = readyQueue.getProcess().registers[i];    //  Sets the CPU registers to the new PCB registers                  
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


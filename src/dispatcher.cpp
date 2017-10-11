#include "dispatcher.hpp"
#include "PriorityQueue.h"
#include "std.h"

void Dispatcher::dispatch(cpu* CPU, PCB* cProcess, PCB* nProcess)
{
    for(int i = 0; i < cpu_cycle; i++)
    {
        wait = CPU.decode_and_execute();
        if(wait)
        {
            switchOut(CPU, cProcess);
            switchIn(CPU, nProcess);
            break;
        }
    }
}

void Dispatcher::switchIn(cpu* CPU, PCB* nProcess)
{
    for(int i = 0; i < CPU.registers.size(); i++)
    {
        CPU.registers[i] = nProcess.registers[i];
        readyQueue.Q.pop();                             //  Removes the Current Running Process        
        //CPU.CurrentProcess = readyQueue.getProcess(); //  Assigns the new First Process in the Ready Queue to the current Process in the CPU
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


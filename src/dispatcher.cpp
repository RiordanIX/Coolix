#include "dispatcher.hpp"
#include "std.h"

void Dispatcher::dispatch(cpu* CPU, PCB* cProcess, PCB* nProcess)
{
    for(int i = 0; i < cpu_cycle; i++)
    {
        CPU.decode_and_execute();
        // if (wait) then switchOut and Break;
        //switchOut(CPU, cProcess);
        //switchIn(CPU, nProcess);
    }
}

void Dispatcher::switchIn(cpu* CPU, PCB* nProcess)
{
    for(int i = 0; i < CPU.registers.size(); i++)
    {
        CPU.registers[i] = nProcess.registers[i];
    }
}

void Dispatcher::switchOut(cpu* CPU, PCB* cProcess)
{
     for(int i = 0; i < CPU.registers.size(); i++)
    {
        cProcess.registers[i] = CPU.registers[i];
    }
}


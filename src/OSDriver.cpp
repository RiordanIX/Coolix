#include "OSDriver.hpp"

OSDriver::OSDriver()
{
    
}

OSDriver::~OSDriver()
{
    
}

void OSDriver::RunOS(std::string fileName, Disk& disk)
{
    loader.readFromFile("DataFile", disk);
    while(readyQueue.Q.size() > 0)
    {
        for(int i = 0; i < cpu_cycle; i++)
        {
            wait = CPU.decode_and_execute();
            //  Call Scheduler
            Dispatch.dispatch(CPU, readyQueue.getProcess(),)
            if(!wait)
            {
                //----------- Scheduler Work -----------------
                //  Long term scheduler puts into RAM?
                //  Short term scheduler dispatches from RAM 
            }
            
        }
        
        
    }
    
}
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
        //  Call Long Term Scheduler
        for(int i = 0; i < cpu_cycle; i++)
        {
            wait = CPU.decode_and_execute();
            //  Call Short Term Scheduler
            Dispatch.dispatch(CPU, readyQueue.getProcess(), wait);
        }
    }
    
}
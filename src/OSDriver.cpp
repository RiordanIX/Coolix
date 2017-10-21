#include "OSDriver.hpp"

OSDriver::OSDriver()
{

}

OSDriver::~OSDriver()
{

}

void OSDriver::run(std::string fileName)
{
    loader.readFromFile(fileName);  //  Load to Disk
    while(readyQueue.Size() > 0)
    {
        run_longts();
        run_cpu();
        run_shortts();
    }
}


void OSDriver::run_cpu()
{
    currentState = CPU.decode_and_execute();
    current_cycle++;
}


void OSDriver::run_longts()
{
    ltSched.DiskToRam();                //  Populate RAM and ReadyQueue  
    ltSched.ReadyToWait();              //  Checks to see if any process in the Ready Queue should be moved to Waiting Queue, then moves it
    ltSched.WaitToReady();              //  Checks to see if any process in the Waiting Queue should be moved to Ready Queue, then moves it
}

void OSDriver::run_shortts()
{
    Dispatch.dispatch(CPU, readyQueue.getProcess(), currentState, current_cycle, cpu_cycle);
    if(current_cycle >= cpu_cycle)
        current_cycle = 0;
}


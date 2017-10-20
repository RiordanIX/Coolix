#include "OSDriver.hpp"

OSDriver::OSDriver()
{

}

OSDriver::~OSDriver()
{

}

void OSDriver::run(std::string fileName)
{

    loader.readFromFile(fileName);
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


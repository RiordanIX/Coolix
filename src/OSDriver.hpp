#pragma once

#define DEFAULT_CPU_CYCLE_TIME  5;

//#include "FIFO.h"
#include "loader.hpp"
#include "cpu.hpp"
#include "dispatcher.hpp"
//#include short term scheduler
//#include long term scheduler



class OSDriver
{
public:
    OSDriver();
    ~OSDriver();
    
    bool wait;
    int cpu_cycle = DEFAULT_CPU_CYCLE_TIME;
    
    loader Loader;  
    Dispatcher Dispatch;
    cpu CPU;
    
    void Run();  // Runs the OS
    
}

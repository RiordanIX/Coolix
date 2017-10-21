#pragma once

#define DEFAULT_CPU_CYCLE_TIME  5;

//#include "FIFO.h"
#include "loader.hpp"
#include "cpu.hpp"
#include "dispatcher.hpp"
#include "pcb.hpp"
#include "PriorityQueue.h"
//#include short term scheduler
//#include long term scheduler

extern PCB process_list;
extern PriorityQueue readyQueue;

class OSDriver
{
public:
    OSDriver();
    ~OSDriver();

    status currentState;
    int cpu_cycle = DEFAULT_CPU_CYCLE_TIME;
    int current_cycle = 0;
    
    loader Loader = loader();
    Dispatcher Dispatch = Dispatcher();;
    cpu CPU = cpu();
    LongTerm ltSched = LongTerm();

    void run(std::string fileName);  // Runs the OS
    void run_cpu(); 
    void run_shortts();
    void run_longts();
};


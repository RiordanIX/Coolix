#pragma once

#define DEFAULT_CPU_CYCLE_TIME  50

//#include "FIFO.h"
#include "loader.hpp"
#include "cpu.hpp"
#include "dispatcher.hpp"
#include "pcb.hpp"
#include "PriorityQueue.h"
#include "LongTerm.h"
//#include short term scheduler
//#include long term scheduler

//extern PCB process_list;
//extern PriorityQueue readyQueue;

class OSDriver {
public:
    OSDriver();
    ~OSDriver();

    int cpu_cycle;
    int current_cycle;

    loader ldr;
    Dispatcher Dispatch;
    cpu CPU;
    LongTerm ltSched;

    void run(std::string fileName);  // Runs the OS
    void run_cpu();
    void run_shortts();
    void run_longts();
};


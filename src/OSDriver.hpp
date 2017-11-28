#pragma once

#define DEFAULT_CPU_CYCLE_TIME  50

// TODO: change type of queue to be selected at runtime
//#include "FIFO.hpp"
#include "loader.hpp"
#include "cpu.hpp"
#include "dispatcher.hpp"
#include "pcb.hpp"
#include "PriorityQueue.hpp"
#include "LongTerm.hpp"
#include "CpuPool.hpp"


class OSDriver {
public:
    OSDriver();

    int cpu_cycle;
    int current_cycle;

    loader ldr;
    Dispatcher Dispatch;
    cpu CPU;
    LongTerm ltSched;

    void run(std::string fileName);  // Runs the OS
//    void run_cpu();
    void run_shortts(cpu *CPU);
    void run_longts();
};


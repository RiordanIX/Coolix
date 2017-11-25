#pragma once

#define DEFAULT_CPU_CYCLE_TIME  50

//#include "FIFO.hpp"
#include "PriorityQueue.hpp"
#include "loader.hpp"
#include "mmu.hpp"
#include "cpu.hpp"
#include "dispatcher.hpp"
#include "pcb.hpp"
#include "LongTerm.hpp"
#include "ShortTerm.hpp"

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
    LongTerm ltSched;
	ShortTermScheduler StSched;

    void run(std::string fileName);  // Runs the OS

private:
    void run_cpu(cpu CPU);
    void run_shortts(cpu CPU);
    void run_longts();
	void print_error(PCB* p);
};


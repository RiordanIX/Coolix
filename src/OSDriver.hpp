#pragma once

#define DEFAULT_CPU_CYCLE_TIME  50

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

#include "FIFO.hpp"
#include "loader.hpp"
#include "cpu.hpp"
#include "dispatcher.hpp"
#include "pcb.hpp"
#include "PriorityQueue.hpp"
#include "LongTerm.hpp"
#include "ShortTerm.hpp"
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
	int totalJobs;
	std::vector<PCB *> RunningQ;
    loader ldr;
    Dispatcher Dispatch;
    LongTerm ltSched;
	ShortTermScheduler StSched;

    void run(std::string fileName);  // Runs the OS

private:
  //  void run_cpu(cpu* CPU);
    void run_shortts(cpu * CPU);
    void run_longts();
	void print_error(PCB* p);
	
};


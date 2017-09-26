#pragma once
#include "instruct.hpp"
#include "string.h"
#include <string>
#include <vector>

typedef struct PCB
{

public:
    unsigned int cpuid;
    instruct_t programCounter, code_size;
    Process_State state;
    
    // "pointer to parent (if this process is spawned, else ‘null’)"
    PCB* parent;
    
    // {running, ready, blocked, new}
    string status;
    
    //  pointer to ‘ready-list of active processes’ or ‘resource-list on blocked processes’ of the process, extracted from the 
    vector<PCB> Ready_Queue;
     
    //  "PC, Registers, Permissions, Buffers, Cache, Active Pages/Blocks"
    struct Process_State
    {
        CPU pc;
        //  [insert permissions variable here]
        //  [insert active pages/blocks here]
        vector<instruct_t> buffers, caches;
        vector<Process_Registers> registers;
    }
    
    // "Accumulators, Index, General"
    struct Process_Registers
    {
        instruct_t R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12;
    }
    
    //  "burst-time, priority, queue-type, time-slice, remain-time"
    struct Process_Sched
    {
        unsigned int tBurst, priority, tSlice, tRemain;
        //  [insert queue type here]
    }
    
    // "cpu-time, time-limit, time-delays, start/end times, io-times"
    struct Process_Accounts
    {
        unsigned int tCPU, tLimit, tDelay, tStart, tEnd, tIO;
    }
    
    //  "page-table-base, pages, page-size, base-registers – logical/physical map, limit-reg"
    struct Process_Memories
    {
        //  [Insert Anything Here]
    }
    
    //  "child-procid, child-code-pointers"
    struct Process_Progeny
    {
        unsigned int childId;
        PCB* child;
    }
    
    //  "file-pointers, io-devices – unitclass, unit#, open-file-tables"
    struct Process_Resources
    {
        //  [insert File-Pointers here]
        //  [insert io devices and other stuff here]
    }
    
    
	PCB();
	~PCB();
	void SetPriority(int priorityIn);
    
private:

};


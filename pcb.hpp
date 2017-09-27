#pragma once
#include "instruct.hpp"
#include "disk.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef struct PCB
{

public:
    instruct_t cpuid;
    instruct_t programCounter, code_size;
    Process_State state;
    Process_Sched sched;
    Process_Accounts acct;
    Process_Memories mem;
    Process_Progeny children;
    Process_Resources resources;

    // "pointer to parent (if this process is spawned, else ‘null’)"
    PCB* parent;

    // {running, ready, blocked, new}
    enum status {
        READY,
        RUNNING,
        BLOCKED,
        NEW,
        WAITING,
        TERMINATED
    };

    //  pointer to ‘ready-list of active processes’ or ‘resource-list on blocked processes’ of the process, extracted from the
    vector<PCB> Ready_Queue;

    //  "PC, Registers, Permissions, Buffers, Cache, Active Pages/Blocks"

    // "Accumulators, Index, General"
    struct Process_Registers
    {
        instruct_t R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12;
    };

    struct Process_State
    {
        // CPU pc; // Only have one cpu instance, which is in virt_comp
        //  [insert permissions variable here] -- unneeded?
        //  [insert active pages/blocks here]
        vector<instruct_t> buffers, caches;
        Process_Registers registers;
    };

    //  "burst-time, priority, queue-type, time-slice, remain-time"
    struct Process_Sched
    {
        instruct_t tBurst, priority, tSlice, tRemain;
        //  [insert queue type here] -- pointers to queues
    };

    // "cpu-time, time-limit, time-delays, start/end times, io-times"
    struct Process_Accounts
    {
        instruct_t tCPU, tLimit, tDelay, tStart, tEnd, tIO;
    };

    //  "page-table-base, pages, page-size, base-registers – logical/physical map, limit-reg"
    struct Process_Memories
    {
        // virtual memory not implemented yet
    };

    //  "child-procid, child-code-pointers"
    struct Process_Progeny
    {
        instruct_t childId;
        PCB* child;
    };

    //  "file-pointers, io-devices – unitclass, unit#, open-file-tables"
    struct Process_Resources
    {
        vector<ofstream> openfiles;
        Disk disk;

        //  [insert io devices and other stuff here]
    };


	void SetPriority(int priorityIn);

private:

};


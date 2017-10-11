#pragma once
#include "instruct.hpp"
#include "disk.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class PCB
{

public:

    // {running, ready, blocked, new}
    enum status {
        READY,
        RUNNING,
        BLOCKED,
        NEW,
        WAITING,
        TERMINATED
    };
	
	/* enum buffType
	{
		INSTRUCTION,
		INPUT,
		OUTPUT,
		TEMP,
		END
	}; */
	
	enum resourceType {
		DISK,
		STDOUT,
		KEYBOARD,
		SHMEM
	};
	
	PCB(instruct_t id, instruct_t address, instruct_t *pc, instruct_t instruct, instruct_t inp, instruct_t out, instruct_t temp, int p)
	{
		pid = id;
		currentStatus = READY;
		programCounter = pc; // PC should be a pointer because it points to (i.e. holds the address of) the instruction to be executed.
		
		diskAddress = address;
		ramAddress = address;	// virtual memory info to be added later
		
		buffSizes[0] = instruct;
		buffSizes[1] = inp;
		buffSizes[2] = out;
		buffSizes[3] = temp;
		
		priority = p;
	}
	void SetPriority(int priorityIn);
	instruct_t get_inp_address();
	instruct_t get_out_address();
	instruct_t get_temp_address();

private:
	int pid;
	status currentStatus;
	resourceType resource_held;
	int priority;
	
	instruct_t diskAddress, ramAddress;
	
    instruct_t cpuid;
    instruct_t *programCounter //code_size;
	
	vector<instruct_t> registers;
	vector<instruct_t> buffSizes;
};


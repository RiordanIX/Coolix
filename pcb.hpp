#pragma once
#include "instruct.hpp"
#include "disk.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

    enum status {
        READY,
        RUNNING,
        BLOCKED,
        NEW,
        WAITING,
        TERMINATED
    };
	
	enum buffType
	{
		INSTRUCTION,
		INPUT,
		OUTPUT,
		TEMP
	};
	
	enum resourceType {
		DISK,
		STDOUT,
		KEYBOARD,
		SHMEM
	};

class PCB
{

public:
	PCB(instruct_t id, std::size_t raddress, std::size_t daddress std::size_t pc, instruct_t instruct, instruct_t inp, instruct_t out, instruct_t temp, int p)
	{
		pid = id;
		currentStatus = READY;
		programCounter = pc; // PC should be a pointer because it points to (i.e. holds the address of) the instruction to be executed.
		
		diskAddress = daddress;
		ramAddress = raddress;	// virtual memory info to be added later
		
		sectionSizes[INSTRUCTION] = instruct;
		sectionSizes[INPUT] = inp;
		sectionSizes[OUTPUT] = out;
		sectionSizes[TEMP] = temp;
		
		priority = p;
	}
	
	// GETTERS
	int get_priority() { return priority; }
	unsigned int get_pid() { return pid; }
	instruct_t get_ram_address() { return ramAddress; }
	instruct_t get_disk_address() { return diskAddress; }
	instruct_t get_inp_address() { return ramAddress + sectionSizes[INSTRUCTION]; }
	instruct_t get_out_address() { return ramAddress + sectionSizes[INSTRUCTION] + sectionSizes[INPUT]; }
	instruct_t get_temp_address() { return ramAddress + sectionSizes[INTRUCTION] + sectionSizes[INPUT] + sectionSizes[OUTPUT]; }
	instruct_t get_end_address() { return ramAddress + sectionSizes[INTRUCTION] + sectionSizes[INPUT] + sectionSizes[OUTPUT] + sectionSizes[TEMP]; }
	
	
	// SETTERS
	void SetPriority(int priorityIn);

private:
	unsigned int pid;
	status currentStatus;
	resourceType resource_held;
	int priority;
	
	std::size_t diskAddress, ramAddress;
	
    // instruct_t cpuid;
    std::size_t programCounter //code_size;
	
	vector<instruct_t> registers;
	vector<instruct_t> sectionSizes;
};


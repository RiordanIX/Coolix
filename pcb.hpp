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
	int pid;
	status currentStatus;
	int priority;
	
	instruct_t diskAddress, ramAddress;
	
    instruct_t cpuid;
    instruct_t programCounter, code_size;
	
	vector<instruct_t> registers;
	vector<instruct_t> buffSizes;

    // {running, ready, blocked, new}
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
		INSTRUCTION = 0,
		INPUT,
		OUTPUT,
		TEMP,
		END
	};

	PCB(instruct_t id, instruct_t address, instruct_t instruct, instruct_t inp, instruct_t out, instruct_t temp, int p)
	{
		pid = id;
		currentStatus = status::READY;
		
		diskAddress = address;
		ramAddress = address;
		
		buffSizes[buffType::INSTRUCTION] = instruct;
		buffSizes[buffType::INPUT] = inp;
		buffSizes[locationType::OUTPUT] = out;
		buffSizes[buffType::TEMP] = temp;
		
		priority = p;
	}
	void SetPriority(int priorityIn);
	instruct_t get_inp_address();
	instruct_t get_out_address();
	instruct_t get_temp_address();

private:

};


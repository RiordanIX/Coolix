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
    instruct_t cpuid;
    instruct_t programCounter, code_size;
	int pid;
	status currentStatus;
	int priority;
	vector<instruct_t> registers;
	vector<instruct_t> locations;

    // {running, ready, blocked, new}
    enum status {
        READY,
        RUNNING,
        BLOCKED,
        NEW,
        WAITING,
        TERMINATED
    };
	
	enum locationType
	{
		INSTRUCTION = 0,
		INPUT,
		OUTPUT,
		TEMP,
		END
	};

	PCB(instruct_t id, instruct_t instruct, instruct_t inp, instruct_t out, instruct_t temp, int p)
	{
		pid = id;
		
		locations[locationType::INSTRUCTION] = instruct;
		locations[locationType::INPUT] = inp;
		locations[locationType::OUTPUT] = out;
		locations[locationType::TEMP] = temp;
		
		priority = p;
	}
	void SetPriority(int priorityIn);

private:

};


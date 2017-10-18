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
		NONE,
		DISK,
		STDOUT,
		KEYBOARD,
		SHMEM
	};


class PCB
{

public:
	PCB(instruct_t id, std::size_t raddress, std::size_t daddress std::size_t pc, instruct_t instruct, instruct_t inp, instruct_t out, instruct_t temp, int wait, int cycle, int p)
	{
		pid = id;
		currentStatus = READY;
		programCounter = pc;

		diskAddress = daddress; // ???
		ramAddress = raddress;	// virtual memory info to be added later

		wait_time = wait;
		cycle_time = cycle;

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
	instruct_t get_temp_address() { return ramAddress + sectionSizes[INSTRUCTION] + sectionSizes[INPUT] + sectionSizes[OUTPUT]; }
	instruct_t get_end_address() { return ramAddress + sectionSizes[INSTRUCTION] + sectionSizes[INPUT] + sectionSizes[OUTPUT] + sectionSizes[TEMP]; }
	int get_wait_time() { return wait_time; }
	int get_cycle_time() { return cycle_time; }
	void get_registers (vector<instruct_t> dest);
	status get_resource_status() { return resource_held; }
	status get_status() { return currenStatus;}
	std::size_t get_program_counter()	{ return programCounter; }



	// SETTERS
	void set_priority(int priorityIn);
	void stash_registers(vector<instruct_t> source);
	void acquire_resource(status code);
	void set_status(status code);
	void set_wait_time(int newtime);
	void set_cycle_time(int newtime);
	std::size_t set_program_counter(std::size_t new_pc);


private:
	unsigned int pid;
	status currentStatus;
	resourceType resource_held = NONE;
	int priority;
	int wait_time;
	int cycle_time;

	std::size_t diskAddress, ramAddress;

    // instruct_t cpuid;
    std::size_t programCounter //code_size;

	vector<instruct_t> registers;
	vector<instruct_t> sectionSizes;
};

// Define the PCB List after definition
vector<PCB> process_list;


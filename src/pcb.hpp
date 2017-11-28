#pragma once
#include "instruct.hpp"
#include "disk.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum status
{
	READY,
	RUNNING,
	BLOCKED,
	NEW,
	WAITING,
	TERMINATED
};

enum section
{
	INSTRUCTION,
	INPUT,
	OUTPUT,
	TEMP
};

enum resourceType
{
	NONE,
	DISK_IO,
	STDOUT,
	KEYBOARD,
	SHMEM
};


class PCB
{

public:
	PCB(int id, std::size_t daddress, std::size_t instruct, std::size_t inp, std::size_t out, std::size_t temp, int p) :
			pid(id),
			currentStatus(status::NEW),
			resource_held(resourceType::NONE),
			priority(p),
			wait_time(0),
			diskAddress(daddress),
			ramAddress(0xDEADBEEF),
			programCounter(0),
			registers(16, 0),
			sectionSizes(4,0),
			hit(0),
			miss(0)
		{
		sectionSizes[section::INSTRUCTION] = instruct;
		sectionSizes[section::INPUT] = inp;
		sectionSizes[section::OUTPUT] = out;
		sectionSizes[section::TEMP] = temp;
	}

	// GETTERS
	int get_priority() { return priority; }
	unsigned int get_pid() { return pid; }
	std::size_t get_ram_address() { return ramAddress; }
	std::size_t get_disk_address() { return diskAddress; }
	std::size_t get_inp_address() { return sectionSizes[INSTRUCTION]; }
	std::size_t get_out_address() { return sectionSizes[INSTRUCTION] + sectionSizes[INPUT]; }
	std::size_t get_temp_address() { return sectionSizes[INSTRUCTION] + sectionSizes[INPUT] + sectionSizes[OUTPUT]; }
	std::size_t get_end_address() { return sectionSizes[INSTRUCTION] + sectionSizes[INPUT] + sectionSizes[OUTPUT] + sectionSizes[TEMP]; }
	int get_wait_time() { return wait_time; }
	int get_start_time() { return start_time; }
	int get_end_time() { return end_time; }
	int get_run_time() { return (start_time - end_time); }
	int get_cycle_start_time() { return cycle_start_time; }

	std::vector<instruct_t> get_registers ();
	resourceType get_resource_status() { return resource_held; }
	status get_status() { return currentStatus;}
	std::size_t get_program_counter()	{ return programCounter; }



	// SETTERS
	void set_priority(int priorityIn);
	void set_registers(std::vector<instruct_t> source);
	//void acquire_resource(status code);
	void set_status(status code);
	void set_wait_time(int newtime);
	void set_start_time(int startIn);
	void set_end_time(int endIn);
	void set_cycle_start_time(int cycleIn);

	void set_ram_address(std::size_t address);
	void set_program_counter(std::size_t new_pc);
	void increment_PC() { programCounter += 4; }
	void cache_hit() { hit++;}
	void cache_miss() { miss++;}
	void cache_stats() {
		printf("Process %u:\tHit: %d\tMiss: %d\n",pid, hit, miss);
	}


private:
	unsigned int pid;
	status currentStatus;
	resourceType resource_held;
	int priority;
	int wait_time;
	int start_time;
	int end_time;
	int run_time;
	int cycle_start_time; // number of cycles ran before this PCB is pushed into RAM


	std::size_t diskAddress, ramAddress;

    // instruct_t cpuid;
	std::size_t programCounter; //code_size

	std::vector<instruct_t> registers;
	std::vector<instruct_t> sectionSizes;
	// for Cache hit and miss
	int hit, miss;
};



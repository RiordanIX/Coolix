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
			cycle_time(0),
			diskAddress(daddress),
			ramAddress(0xDEADBEEF),
			programCounter(0),
			registers(16, 0),
			sectionSizes(4,0)
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
	std::size_t get_inp_address() { return ramAddress + sectionSizes[INSTRUCTION]; }
	std::size_t get_out_address() { return ramAddress + sectionSizes[INSTRUCTION] + sectionSizes[INPUT]; }
	std::size_t get_temp_address() { return ramAddress + sectionSizes[INSTRUCTION] + sectionSizes[INPUT] + sectionSizes[OUTPUT]; }
	std::size_t get_end_address() { return ramAddress + sectionSizes[INSTRUCTION] + sectionSizes[INPUT] + sectionSizes[OUTPUT] + sectionSizes[TEMP]; }
	int get_wait_time() { return wait_time; }
	int get_cycle_time() { return cycle_time; }
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
	void set_cycle_time(int newtime);
	void set_ram_address(std::size_t address);
	void set_program_counter(std::size_t new_pc);


private:
	unsigned int pid;
	status currentStatus;
	resourceType resource_held;
	int priority;
	int wait_time;
	int cycle_time;

	std::size_t diskAddress, ramAddress;

    // instruct_t cpuid;
	std::size_t programCounter; //code_size

	std::vector<instruct_t> registers;
	std::vector<instruct_t> sectionSizes;
};



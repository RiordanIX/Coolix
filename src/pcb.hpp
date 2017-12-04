#pragma once
#include "instruct.hpp"
#include "disk.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
// we have 4 bytes per instruction, 4 instructions per frame.
#define INST_SIZE 4
#define PAGE_SIZE 4*4
extern std::size_t clock_Tick;
using namespace std::chrono;
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
public:	PCB(std::size_t id, std::size_t daddress, std::size_t instruct, std::size_t inp, std::size_t out, std::size_t temp, std::size_t p) :
	pid(id),
	currentStatus(status::NEW),
	resource_held(resourceType::NONE),
	priority(p),
	wait_time(0),
	wait_start_clock(clock_Tick),
	wait_end_clock(0),
	wait_time_clock(0),
	start_time_clock(clock_Tick),
	end_time_clock(0),
	exec_time_clock(0),
	page_fault_start_clock(0),
	page_fault_end_clock(0),
	page_fault_time_clock(0),
	diskAddress(daddress),
	ramAddress(0xDEADBEEF),
	programCounter(0),
	registers(16, 0),
	sectionSizes({instruct, inp, out, temp}),
	pageTable(((instruct + inp + out + temp) / (PAGE_SIZE)) + 2)

{ }


		// GETTERS
		std::size_t get_priority() { return priority; }
		std::size_t get_pid() { return pid; }
		std::size_t get_ram_address() { return ramAddress; }
		std::size_t get_disk_address() { return diskAddress; }
		std::size_t get_inp_address() { return sectionSizes[INSTRUCTION]; }
		std::size_t get_out_address() { return sectionSizes[INSTRUCTION] + sectionSizes[INPUT]; }
		std::size_t get_temp_address() { return sectionSizes[INSTRUCTION] + sectionSizes[INPUT] + sectionSizes[OUTPUT]; }
		std::size_t get_end_address() { return sectionSizes[INSTRUCTION] + sectionSizes[INPUT] + sectionSizes[OUTPUT] + sectionSizes[TEMP]; }
		std::size_t get_wait_time() { return wait_time; }
		std::size_t get_start_time() { return start_time; }
		std::size_t get_end_time() { return end_time; }
		std::size_t get_run_time() { return run_time; }
		std::size_t get_cycle_start_time() { return cycle_start_time; }
		bool get_waitformmu() { return waitformmu; }
		std::size_t get_lastRequestedPage() { return lastRequestedPage; }

		std::vector<instruct_t> get_registers();
		resourceType get_resource_status() { return resource_held; }
		status get_status() { return currentStatus; }
		std::size_t get_program_counter() { return programCounter; }

		std::size_t get_page_table_length() { return pageTable.pages.size(); }
		std::size_t get_frame(std::size_t pageNumber);
		std::size_t pop_lru_page() { std::size_t lru = page_stack.back(); page_stack.pop_back(); return lru; }
		std::pair<bool, size_t> get_page_table_entry(std::size_t pageNumber);
		bool is_valid_page(size_t frame);
		std::size_t get_cpuid() { return cpuId; }


		// SETTERS

		void set_priority(std::size_t priorityIn);
		void set_registers(std::vector<instruct_t> source);
		//void acquire_resource(status code);
		void set_status(status code);
		void set_wait_time(std::size_t newtime);
		void set_start_time(std::size_t startIn);
		void set_cycle_start_time(std::size_t cycleIn);

		void set_ram_address(std::size_t address);
		void set_program_counter(std::size_t new_pc);
		void increment_PC() { programCounter += 4; }

		void set_page_table_entry(std::size_t entry, bool valid, std::size_t frame);
		void update_page_stack(std::size_t pageNumber);
		void set_waitformmu(bool mmuwait) { waitformmu = mmuwait; }
		void set_lastRequestedPage(std::size_t pageNumber) { lastRequestedPage = pageNumber; }
		void set_cpuid(std::size_t id) { cpuId = id; }

		//	STATISTICS
		void increment_io_count() { io_count++; }
		std::size_t get_io_count() { return io_count; }
		void set_max_frames(std::size_t framesIn) { max_frames = framesIn; }
		std::size_t get_max_frames() { return max_frames; }
		void increment_page_fault_count() { page_fault_count++; }
		std::size_t get_page_fault_count() { return page_fault_count; }
		void increment_page_fault_service() { fault_service_time++; }
		std::size_t get_page_fault_service() { return fault_service_time; }
		void increment_cache_hit() { cache_hit++; }
		void increment_cache_miss() { cache_miss++; }
		std::size_t get_cache_hit() { return cache_hit; }
		std::size_t get_cache_miss() { return cache_miss; }

		//	Counting Cycles
		void increment_wait() { wait_cycles++; }
		std::size_t get_wait() { return wait_cycles; }
		void increment_run() { run_cycles++; }
		std::size_t get_run() { return run_cycles; }

		//	Counting Microseconds
		void set_wait_start_clock() { wait_start_clock = clock_Tick; }
		void set_wait_end_clock() {wait_end_clock = clock_Tick;
									wait_time_clock += (wait_end_clock - wait_start_clock);
		}
		void set_start_clock() {start_time_clock = clock_Tick;}//time when loaded to newqueue
		void set_end_clock() {end_time_clock = clock_Tick;}//when pcb terminated
		void set_page_fault_start_clock() { page_fault_start_clock = clock_Tick; }
		void set_page_fault_end_clock()
		{
			page_fault_end_clock = clock_Tick;
			page_fault_time_clock += (page_fault_end_clock - page_fault_start_clock);
		}
		std::size_t get_wait_time_clock() { return wait_time_clock; }

		std::size_t get_exec_time_clock() { return (end_time_clock - start_time_clock); }

		std::size_t get_page_fault_time_clock() { return page_fault_time_clock; }


private:
	std::size_t pid;
	status currentStatus;
	resourceType resource_held;
	std::size_t priority;
	std::size_t wait_time;
	std::size_t start_time;
	std::size_t end_time;
	std::size_t run_time;
	std::size_t cpuId;

	std::size_t wait_start_clock;
	std::size_t wait_end_clock;
	std::size_t wait_time_clock;
	std::size_t start_time_clock;
	std::size_t end_time_clock;
	std::size_t exec_time_clock;
	std::size_t page_fault_start_clock;
	std::size_t page_fault_end_clock;
	std::size_t page_fault_time_clock;
	//  STATISTICS  [MAX_RAM_USED = (max_frames * PAGE_SIZE)] Insert in pcb.hpp under 115 private:
	std::size_t run_cycles, wait_cycles;
	std::size_t io_count;
	std::size_t max_frames; // Max Frames in RAM
	std::size_t cache_hit;
	std::size_t cache_miss;
	std::size_t page_fault_count;
	std::size_t fault_service_time;

	std::size_t cycle_start_time; // number of cycles ran before this PCB is pushed into RAM
	bool waitformmu; //check to see if process is waiting for mmu
	std::size_t lastRequestedPage; //keep track of last page process request when pagefault occured

	std::size_t diskAddress, ramAddress;
	// instruct_t cpuid;
	std::size_t programCounter; //code_size
	std::vector<instruct_t> registers;
	std::vector<std::size_t> sectionSizes;
	std::deque<std::size_t> page_stack;		//stack of most recently used pages

public:
	struct PageTable
	{
		std::vector<std::pair<bool, instruct_t>> pages;
		PageTable(std::size_t size) : pages(size)
		{
			for (auto it = pages.begin(); it != pages.end(); it++)
			{
				(*it).first = false;
				(*it).second = -1;
			}
		}
		std::size_t size() { return pages.size(); }
	};
	PageTable pageTable;
};



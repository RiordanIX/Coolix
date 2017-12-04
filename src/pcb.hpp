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
extern long int clock_Tick;
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
public:	PCB(int id, std::size_t daddress, std::size_t instruct, std::size_t inp, std::size_t out, std::size_t temp, int p) :
	pid(id),
	currentStatus(status::NEW),
	resource_held(resourceType::NONE),
	priority(p),
	wait_time(0),
	diskAddress(daddress),
	ramAddress(0xDEADBEEF),
	programCounter(0),
	registers(16, 0),
	sectionSizes(4, 0),
	pageTable(((instruct + inp + out + temp) / (PAGE_SIZE)) + 2),
	page_fault_start_clock(0),
	page_fault_end_clock(0),
	page_fault_time_clock(0),
	exec_time_clock(0),//entire time
	end_time_clock(0),//end pcb time stamp
	wait_time_clock(0),//total wait
	wait_start_clock(0),//start wait stamp
	wait_end_clock(0)//end wait stamp
{
	sectionSizes[section::INSTRUCTION] = instruct;
	sectionSizes[section::INPUT] = inp;
	sectionSizes[section::OUTPUT] = out;
	sectionSizes[section::TEMP] = temp;
	page_fault_time_clock = 0;
	start_time_clock = clock_Tick;
	wait_start_clock = clock_Tick;
	//Give the page table enough pages to fit the process
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
		long int get_wait_time() { return wait_time; }
		long int get_start_time() { return start_time; }
		long int get_end_time() { return end_time; }
		long int get_run_time() { return run_time; }
		long int get_cycle_start_time() { return cycle_start_time; }
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
		int get_cpuid() { return cpuId; }


		// SETTERS

		void set_priority(int priorityIn);
		void set_registers(std::vector<instruct_t> source);
		//void acquire_resource(status code);
		void set_status(status code);
		void set_wait_time(int newtime);
		void set_start_time(int startIn);
		void set_cycle_start_time(int cycleIn);

		void set_ram_address(std::size_t address);
		void set_program_counter(std::size_t new_pc);
		void increment_PC() { programCounter += 4; }

		void set_page_table_entry(std::size_t entry, bool valid, std::size_t frame);
		void update_page_stack(std::size_t pageNumber);
		void set_waitformmu(bool mmuwait) { waitformmu = mmuwait; }
		void set_lastRequestedPage(std::size_t pageNumber) { lastRequestedPage = pageNumber; }
		void set_cpuid(int id) { cpuId = id; }

		//	STATISTICS
		void increment_io_count() { io_count++; }
		int get_io_count() { return io_count; }
		void set_max_frames(int framesIn) { max_frames = framesIn; }
		int get_max_frames() { return max_frames; }
		void increment_page_fault_count() { page_fault_count++; }
		int get_page_fault_count() { return page_fault_count; }
		void increment_page_fault_service() { fault_service_time++; }
		int get_page_fault_service() { return fault_service_time; }
		void increment_cache_hit() { cache_hit++; }
		void increment_cache_miss() { cache_miss++; }
		int get_cache_hit() { return cache_hit; }
		int get_cache_miss() { return cache_miss; }

		//	Counting Cycles
		void increment_wait() { wait_cycles++; }
		int get_wait() { return wait_cycles; }
		void increment_run() { run_cycles++; }
		int get_run() { return run_cycles; }

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
		long int get_wait_time_clock() { return wait_time_clock; }
		
		long int get_exec_time_clock() { return (end_time_clock - start_time_clock); }
		
		long int get_page_fault_time_clock() { return page_fault_time_clock; }


private:
	unsigned int pid;
	status currentStatus;
	resourceType resource_held;
	int priority;
	int wait_time;
	int start_time;
	int end_time;
	int run_time;
	int cpuId;

	long int wait_start_clock;
	long int wait_end_clock;
	long int wait_time_clock;
	long int start_time_clock;
	long int end_time_clock;
	long int exec_time_clock;
	long int page_fault_start_clock;
	long int page_fault_end_clock;
	long int page_fault_time_clock;
	//  STATISTICS  [MAX_RAM_USED = (max_frames * PAGE_SIZE)] Insert in pcb.hpp under 115 private:
	int run_cycles, wait_cycles;
	int io_count;
	int max_frames; // Max Frames in RAM
	int cache_hit;
	int cache_miss;
	int page_fault_count;
	int fault_service_time;

	int cycle_start_time; // number of cycles ran before this PCB is pushed into RAM
	bool waitformmu; //check to see if process is waiting for mmu
	std::size_t lastRequestedPage; //keep track of last page process request when pagefault occured

	std::size_t diskAddress, ramAddress;
	// instruct_t cpuid;
	std::size_t programCounter; //code_size
	std::vector<instruct_t> registers;
	std::vector<instruct_t> sectionSizes;
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



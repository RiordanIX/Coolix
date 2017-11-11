#include "pcb.hpp"

using std::vector;
using std::size_t;


vector<instruct_t> PCB::get_registers(){
	auto to_return = registers;
	return to_return;
}

void PCB::set_priority(int priorityIn){
	priority = priorityIn;
}

void PCB::set_registers(vector<instruct_t> source){
	int i = 0;
	for (auto it = source.begin(); it != source.end(); it++){
		registers[i] = *it;
		i++;
	}
}

//void PCB::acquire_resource(status code) {
//	resource_held = code;
//}

void PCB::set_status(status code) {
	currentStatus = code;
}

void PCB::set_wait_time(int newtime) {
	wait_time = newtime;
}

void PCB::set_end_time(int endIn)
{
	end_time = endIn;
}

void PCB::set_start_time(int startIn)
{
	start_time = startIn;
}

void PCB::set_cycle_start_time(int cycleIn)
{
	cycle_start_time = cycleIn;
}   
 
void PCB::set_program_counter(size_t new_pc){
	programCounter = new_pc;
}

void PCB::set_ram_address(size_t address){
	ramAddress = address;
}

void set_page_table_entry(std::size_t entry, bool valid, std::size_t frame)
{
	pageTable.pages[entry].first = valid;
	pageTable.pages[entry].second = frame;
}

//PAGE TABLE
std::size_t getFrame(std::size_t pageNumber)
{
	if(pageTable.pages[pageNumber].first)
	{
		return pageTable.pages[pageNumber].second;
	}
}

// GLOBAL VARIABLE
// Define the PCB List after definition
std::vector<PCB> process_list;


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


std::size_t PCB::get_frame(std::size_t pageNumber)
{
	 return pageTable.pages[pageNumber].second; 
}

//PAGE TABLE
std::pair<bool,size_t> PCB::get_page_table_entry(size_t pageNumber)
{
	return pageTable.pages[pageNumber];
}

// returns false if out of bounds.
bool PCB::is_valid_page(size_t frame) {
	if (frame > pageTable.size())
	{
		return false;
	}
	return pageTable.pages[frame].first;
}


void PCB::set_page_table_entry(size_t entry, bool valid, size_t frame)
{
	pageTable.pages[entry].first = valid;
	pageTable.pages[entry].second = frame;
}

void PCB::update_page_stack(size_t pageNumber)
{
	auto contains = std::find(page_stack.begin(), page_stack.end(), pageNumber);
	if(contains != page_stack.end())
		page_stack.erase(contains);

	page_stack.push_front(pageNumber);
}

// GLOBAL VARIABLE
// Define the PCB List after definition
std::vector<PCB> process_list;


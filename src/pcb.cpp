#include "pcb.hpp"


void PCB::get_registers(vector<instruct_t>& dest){
	int i = 0;
	for (auto it = registers.begin(); it != registers.end(); it++) {
		dest[i] = *it;
		i++;
	}
}

void PCB::set_priority(int priorityIn){
	priority = priorityIn;
}

void PCB::stash_registers(vector<instruct_t> source){
	int i = 0;
	for (auto it = source.begin(); it != source.end(); it++){
		registers[i] = *it;
		i++;
	}
}

void PCB::acquire_resource(status code) {
	resource_held = code;
}

void PCB::set_status(status code) {
	currentStatus = code;
}

void PCB::set_wait_time(int newtime) {
	wait_time = newtime;
}

void PCB::set_cycle_time(int newtime){
	cycle_time = newtime;
}

void PCB::set_program_counter(std::size_t new_pc){
	programCounter = new_pc;
}

void PCB::set_ram_addres(std::size_t address){
	ramAddress = address;
}

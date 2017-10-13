#include "pcb.hpp"


void PCB::get_registers(vector<instruct_t> dest){
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

void PCB::acquire_resource(unsigned int code) {
	resource_held = code;
}

void PCB::set_status(unsigned int code) {
	currentStatus = code;
}

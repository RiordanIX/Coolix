#include "ram.hpp"
#include <cstdio>

using std::string;
using std::to_string;
using std::vector;
using std::deque;
using std::size_t;

extern std::mutex frame;

Ram::Ram(size_t size) : _size(size), _space(size, 0) { }


/** Assigns instruction to ram location
 * \throws illegal_allocation_error
 */
void Ram::allocate(size_t location, instruct_t data) {
	debug_printf("Placing %u at location %lu in RAM\n", data, location);
	byte_t local = 0;

	local = byte_t((data & 0xFF000000) >> (8*3));
	allocate(location, local);

	local = byte_t((data & 0x00FF0000) >> (8*2));
	allocate(location + 1, local);

	local = byte_t((data & 0x0000FF00) >> (8*1));
	allocate(location + 2, local);

	local = byte_t((data & 0x000000FF) >> (8*0));
	allocate(location + 3, local);
}


void Ram::allocate(size_t location, byte_t data) {
	if (location < _size) {
		//while (frame.try_lock()) {}
		_space.at(location) = data;
	}
	else {
		throw "Illegal allocation at: " + to_string(location) + ".";
	}
}


/** Returns info about ram state
 *
 */
string Ram::get_info() {
	return "Ram size: " + to_string(_size);
}


/** Returns instruction using index to the first byte in the instruction
 *
 */
instruct_t Ram::get_instruction(size_t index) {
	instruct_t instruct = 0;
	instruct |= ((instruct_t)_space[index+0]) << (8 * 3); // Shift 3 bytes
	instruct |= ((instruct_t)_space[index+1]) << (8 * 2);
	instruct |= ((instruct_t)_space[index+2]) << (8 * 1);
	instruct |= ((instruct_t)_space[index+3]) << (8 * 0);

	return instruct;
}

void Ram::allocate_chunk(size_t location, deque<instruct_t> inst) {
	size_t i = location;
	for (auto it = inst.begin(); it != inst.end(); ++it,i += 4) {
		allocate(i, *it);
	}
}


void Ram::dump_data(char* filename) {
	FILE *file;
	file = fopen(filename, "w");
	for (unsigned int i = 0; i < size(); i +=6*4) {
		for (unsigned int j = i; j < i + 6*4 && j < size(); j+=4) {
			fprintf(file, "%4u: 0x%08x   ", j, get_instruction(j));
			printf("%4u: 0x%08x   ", j, get_instruction(j));
		}
		fprintf(file, "\n");
		printf("\n");
	}
	fclose (file);
}

// GLOBAL VARIABLES
Ram MEM;


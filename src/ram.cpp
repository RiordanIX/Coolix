#include <string>
#include "ram.hpp"

using std::string;
using std::to_string;
using std::vector;
using std::deque;
using std::size_t;

Ram::Ram(long unsigned int size) : _size(size), _space(size, 0) { }


/** Assigns instruction to ram location
 * \throws illegal_allocation_error
 */
void Ram::allocate(long unsigned int location, instruct_t data) {
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


void Ram::allocate(long unsigned int location, byte_t data) {
	if (location < _size) {
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


void Ram::allocate_chunk(long unsigned int location, deque<instruct_t> instructions) {
	size_t i = location;
	while(instructions.size()) {
		allocate(i, instructions.front());
		i += 4;
		instructions.pop_front();
	}
}
void Ram::dump_data() {
	for (unsigned int i = 0; i < size(); i +=6*4) {
		for (unsigned int j = i; j < i + 6*4 && j < size(); j+=4) {
			printf("%4u: 0x%08x   ", j, get_instruction(j));
		}
		printf("\n");
	}
}

// GLOBAL VARIABLES
Ram MEM = Ram(DEFAULT_RAM);


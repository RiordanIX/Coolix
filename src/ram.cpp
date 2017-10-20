#include <string>
#include "ram.hpp"

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
		throw "Illegal allocation at: " + std::to_string(location) + ".";
	}
}

/** Returns info about ram state
 *
 */
std::string Ram::get_info() {
	return "Ram size: " + std::to_string(_size);
}


/** Returns instruction using index to the first byte in the instruction
 *
 */
instruct_t Ram::get_instruction(size_t index) {
	instruct_t instruct = 0;
	instruct |= (instruct_t)_space[index] << (8 * 3); // Shift 3 bytes
	instruct |= (instruct_t)_space[index] << (8 * 2);
	instruct |= (instruct_t)_space[index] << (8 * 1);
	instruct |= (instruct_t)_space[index] << (8 * 0);

	return instruct;
}


#include <string>
#include "ram.hpp"

Ram::Ram(long unsigned int size) : _size(size), _space_(size, 0) { }

/** Assigns instruction to ram location
 * \throws illegal_allocation_error
 */

void Ram::allocate(long unsigned int location, instruct_t data) {
	if (location < _size) {
		_space_.at(location) = data;
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


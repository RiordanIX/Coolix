#include "ram.hpp"

Ram::Ram(unsigned int size=DEFAULT_RAM) : _size(size), _space_(size, 0) { }

/** Assigns instruction to ram location
 * \throws illegal_allocation_error
 */

void Ram::allocate(unsigned int location, instruct_t data) {
	if (location < _size) {
		_space_.at(location) = data;
	}
	else {
		throw "Illegal allocation at: " + str(location) + ".";
	}
}


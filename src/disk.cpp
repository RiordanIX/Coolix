#include <vector>
#include "disk.hpp"
#include "instruct.hpp"

void Disk::allocate(instruct_t data) {
	if (is_full()) {
		_disk[_used] = data;
		_used++;
	}
	else {
		throw "Illegal allocation. Disk is full";
	}
}


instruct_t Disk::read_data(int pos) {
	return _disk[pos];
}


std::string Disk::get_info() {
	return "Disk size: " + std::to_string(_size) + "\nDisk used: " +
			std::to_string(_used);
}


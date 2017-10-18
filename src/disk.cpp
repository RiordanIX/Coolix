#include <vector>
#include "disk.hpp"
#include "instruct.hpp"

void Disk::allocate(byte_t data) {
	if (is_full()) {
		throw "Illegal allocation. Disk is full";
	}
	else {
		_disk[_used] = data;
		_used++;
	}
}

/**
 * Everything in the disk is indexed by a byte address.  This is an overloaded
 * method for assigning an entire instruction to disk instead.  The
 * bit-shifting makes sure we don't lose any data in the process.
 */
void Disk::allocate(instruct_t data) {
	byte_t local = 0;

	local = byte_t((data & 0xFF000000) >> (8*3));
	allocate(local);

	local = byte_t((data & 0x00FF0000) >> (8*2));
	allocate(local);

	local = byte_t((data & 0x0000FF00) >> (8*1));
	allocate(local);

	local = byte_t((data & 0x000000FF) >> (8*0));
	allocate(local);
}



/**
 * Because everything is byte-indexed in the disk, we need to convert
 * everything to an instruction
 */
instruct_t Disk::read_instruction(std::size_t pos) {
	if (pos % 4 != 0) {
		throw "Improper index of an instruction in disk at " +
				std::to_string(pos);
	}
	//0000 0000 0000 0000 0000 0000 1010 1010  << (8*3) =
	//1010 1010 0000 0000 0000 0000 0000 0000

	instruct_t instruct = 0;
	instruct |= (instruct_t)_disk[pos + 0] << (8*3); // 3 bytes
	instruct |= (instruct_t)_disk[pos + 1] << (8*2);
	instruct |= (instruct_t)_disk[pos + 2] << (8*1);
	instruct |= (instruct_t)_disk[pos + 3] << (8*0);

	return instruct;
}


std::string Disk::get_info() {
	return "Disk size: " + std::to_string(_size) + "\nDisk used: " +
			std::to_string(_used);
}


std::vector<byte_t> Disk::read_byte_chunk(int pos, int size) {
	std::vector<byte_t> to_send;
	for ( ; pos < size; ++pos) {
		to_send.push_back(_disk[pos]);
	}
	return to_send;
}


std::vector<instruct_t> Disk::read_instruction_chunk(int pos, int size) {
	std::vector<instruct_t> to_send;
	instruct_t temp;
	for (; pos < size; ++pos) {
		temp = 0;
		temp |= (instruct_t)_disk[pos + 0] << (8*3);
		temp |= (instruct_t)_disk[pos + 1] << (8*2);
		temp |= (instruct_t)_disk[pos + 2] << (8*1);
		temp |= (instruct_t)_disk[pos + 3] << (8*0);
		to_send.push_back(temp);
	}
	return to_send;
}


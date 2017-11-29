#include <vector>
#include "disk.hpp"
#include "instruct.hpp"

using std::size_t;
using std::deque;
using std::string;
using std::to_string;

void Disk::allocate(byte_t data) {
	if (is_full()) {
		throw "Illegal allocation. Disk is full";
	}
	else {
		_disk[_used] = data;
		_used++;
	}
}
void Disk::allocate(size_t location,byte_t data) {
	if (location < _size) {
		//while (frame.try_lock()) {}
		_disk.at(location) = data;
	}
	else {
		throw "Illegal allocation at: " + to_string(location) + ".";
	}
}
void Disk::allocate(size_t location, instruct_t data) {
	byte_t local = 0;

	local = byte_t((data & 0xFF000000) >> (8 * 3));
	allocate(location,local);

	local = byte_t((data & 0x00FF0000) >> (8 * 2));
	allocate(location + 1,local);

	local = byte_t((data & 0x0000FF00) >> (8 * 1));
	allocate(location + 2,local);

	local = byte_t((data & 0x000000FF) >> (8 * 0));
	allocate(location + 3,local);
	debug_printf("Allocated: %#010X\n", this->read_instruction(this->_used - 4));
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
	debug_printf("Allocated: %#010X\n",this->read_instruction(this->_used - 4));
}



/**
 * Because everything is byte-indexed in the disk, we need to convert
 * everything to an instruction
 */
instruct_t Disk::read_instruction(size_t pos) {
	if (pos % 4) {
		throw "Improper index of an instruction in disk at " +
				to_string(pos);
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


string Disk::get_info() {
	return "Disk size: " + to_string(_size) + "\nDisk used: " +
			to_string(_used);
}


deque<byte_t> Disk::read_byte_chunk(size_t pos, size_t size) {
	deque<byte_t> to_send;
	size_t pos_copy = pos;
	for (; pos < pos_copy + size; ++pos) {
		to_send.push_back(_disk[pos]);
	}
	return to_send;
}


deque<instruct_t> Disk::read_instruction_chunk(size_t pos, size_t size) {
	deque<instruct_t> to_send;
	instruct_t temp;
	size_t pos_copy = pos;
	for (; pos < pos_copy + size; pos += 4) {
		temp = 0;
		temp |= ((instruct_t)_disk[pos + 0]) << (8*3);
		temp |= ((instruct_t)_disk[pos + 1]) << (8*2);
		temp |= ((instruct_t)_disk[pos + 2]) << (8*1);
		temp |= ((instruct_t)_disk[pos + 3]) << (8*0);
		to_send.push_back(temp);
	}
	return to_send;
}

Disk DISK;


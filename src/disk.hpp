#pragma once
#include <vector>
#include <deque>
#include <string>
#include "instruct.hpp"
#include "debug.hpp"

// 2048 four byte words.  This makes the disk byte-addressable
#define DEFAULT_DISK 2048*4

/******************************************************************************
 * Disk
 * A virtual disk we use for our main storage space.
 *****************************************************************************/
class Disk {
private:
	std::vector<byte_t> _disk;
	std::size_t _used;
	std::size_t _size;

public:
	Disk(long unsigned int size=DEFAULT_DISK) : _disk(size, 0), _used(0), _size(size){}
	void allocate(byte_t data);
	void allocate(instruct_t data);
	// pos must be the byte position
	instruct_t read_instruction(std::size_t pos);
	byte_t     read_byte(std::size_t pos);
	// pos must be byte indexible and size is the number of bytes
	std::deque<byte_t> read_byte_chunk(std::size_t pos, std::size_t size);
	// pos must be byte indexible and size is the number of BYTES
	std::deque<instruct_t> read_instruction_chunk(std::size_t pos, std::size_t size);

	bool is_full() { return _used == _size;}
	std::size_t get_used() { return _used;}
	std::string get_info();

};


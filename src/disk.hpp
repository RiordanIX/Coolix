#pragma once
#include <vector>
#include <string>
#include "instruct.hpp"

// 2048 four byte words.  This makes the disk byte-addressable
#define DEFAULT_DISK 2048*4

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
	instruct_t read_instruction(int pos);
	byte_t     read_byte(int pos);
	// pos must be byte indexible and size is the number of bytes
	vector<byte_t> read_byte_chunk(int pos, int size);
	// pos must be byte indexible and size is the number of INSTRUCTIONS
	vector<instruct_t> read_instruction_chunk(int pos, int size);

	bool is_full() { return _used == _size;}
	std::string get_info();

};


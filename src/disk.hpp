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
	instruct_t read_instruction(std::size_t pos);
	byte_t     read_byte(std::size_t pos);
	bool is_full() { return _used == _size;}
	std::size_t get_used()	{ return _used;}
	std::string get_info();

};


#pragma once
#include <vector>
#include <deque>
#include "instruct.hpp"

// Default ram size in Bytes. 1024 words of size 4
#define DEFAULT_RAM 1024*4

/******************************************************************************
 * Ram
 * A wrapper for a vector.  Simulates system memory.
 * Ram is indexed by the starting byte.  However, everything should be
 * considered as an instruction for cpu purposes.
 *****************************************************************************/
class Ram {
private:
	std::size_t _size;
	std::vector<byte_t> _space;

public:
	Ram(std::size_t size=DEFAULT_RAM);
	std::size_t size() { return _size;}
	void allocate(std::size_t location, instruct_t data);
	void allocate(std::size_t location, byte_t data);
	// location must be BYTE addressable
	void allocate_chunk(std::size_t location, std::deque<instruct_t> instructions);
	instruct_t get_instruction(std::size_t index);

	std::string get_info();
	void dump_data();

};


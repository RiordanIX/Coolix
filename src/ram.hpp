#pragma once
#include <vector>
#include <deque>
#include "instruct.hpp"

// Default ram size in Bytes
#define DEFAULT_RAM 4096

/**
 * Ram is indexed by the starting byte.  However, everything should be
 * considered as an instruction for cpu purposes.  The reason
 */
class Ram {
private:
	long unsigned int _size;
	std::vector<byte_t> _space;

public:
	Ram(long unsigned int size=DEFAULT_RAM);
	long unsigned int size() { return _size;}
	void allocate(long unsigned int location, instruct_t data);
	void allocate(long unsigned int location, byte_t data);
	// location must be BYTE addressable
	void allocate_chunk(long unsigned int location, std::deque<instruct_t> instructions);
	instruct_t get_instruction(std::size_t index);

	std::string get_info();

};



#pragma once
#include <vector>
#include "instruct.hpp"

// Default ram size in case
#define DEFAULT_RAM 1024

class Ram {
private:
	unsigned int _size;
	std::vector<instruct_t> _space_;

public:
	Ram(unsigned int size=DEFAULT_RAM);
	void allocate(unsigned int location, instruct_t data);

};


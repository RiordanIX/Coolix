#pragma once
#include <vector>
#include "instruct.hpp"

// Default ram size in case
#define DEFAULT_RAM 1024

class Ram {
private:
	long unsigned int _size;
	std::vector<instruct_t> _space_;

public:
	Ram(long unsigned int size=DEFAULT_RAM);
	void allocate(long unsigned int location, instruct_t data);
	std::string get_info();

};


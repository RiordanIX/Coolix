#pragma once
#include <vector>
#include "instruct.hpp"

#define DEFAULT_SIZE 2048

class Disk {
public:
	Disk(int size = DEFAULT_SIZE) : _disk(size, 0) {used = 0;}
	void allocate(instruct_t data);
	instruct_t read_data(int pos);

private:
	std::vector<int> _disk;
	std::size_t used;
	bool _isFull;
};


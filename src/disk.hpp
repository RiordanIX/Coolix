#pragma once
#include <vector>

#define DEFAULT_SIZE 2048

class Disk {
public:
	Disk(int size = DEFAULT_SIZE);
	void allocate(instruct_t data);
	instruct_t read_data(int pos);

private:
	std::vector<int> _disk;
	size_t used;
	bool _isFull;
};


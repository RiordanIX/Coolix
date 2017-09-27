#pragma once
#include <vector>
#include <string>
#include "instruct.hpp"

#define DEFAULT_DISK 2048

class Disk {
private:
	std::vector<int> _disk;
	std::size_t _used;
	std::size_t _size;

public:
	Disk(long unsigned int size=DEFAULT_DISK) : _disk(size, 0), _used(0), _size(size){}
	void allocate(instruct_t data);
	instruct_t read_data(int pos);
	bool is_full() { return _used == _size;}
	std::string get_info();

};


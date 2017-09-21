#include <vector>
#include "disk.hpp"

Disk::Disk(int size)
{
	_disk(size, 0);
	used = 0;
}

void Disk::allocate(instruct_t data)
{
	_disk[used] = data;
	used++;
}

instruct_t Disk::read_data(int pos)
{
	return _disk[pos];
}

#include <vector>
#include "disk.hpp"
#include "instruct.hpp"

void Disk::allocate(instruct_t data)
{
	_disk[used] = data;
	used++;
}

instruct_t Disk::read_data(int pos)
{
	return _disk[pos];
}

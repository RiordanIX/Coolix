#include <iostream>
#include <cstdlib>
#include <string>
#include "../src/loader.hpp"
#include "../src/disk.hpp"
#include "../src/ram.hpp"

int main()
{
	Disk disk;
	Ram ram;
	loader load;

    load.readFromFile("../test_job", disk);
	
	std::cout << disk.get_info() << std::endl;
	
	for(int i = 0; i < disk.get_used(); i++)
	{
		std::cout << std::hex << (int)disk.read_byte(i) << std::endl;
	}
	
	for(int i = 0; i < disk.get_used(); i++)
	{
		ram.allocate(0, disk.read_byte(i));
		ram.get_instruction(i);
	}
}

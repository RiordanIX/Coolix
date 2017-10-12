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
	
	std::cout << "read_byte output" << std::endl;
	for(int i = 0; i < disk.get_used(); i++)
	{
		std::cout << std::hex << (int)disk.read_byte(i) << std::endl;
	}
	
	std::cout << "disk.get_instruction output" << std::endl;
	for(int i = 0; i < disk.get_used(); i += 4)
	{
		std::cout << std::hex << (int)disk.read_instruction(i) << std::endl;
	}
	
	std::cout << "ram.get_instruciton output" << std::endl;
	for(int i = 0; i < disk.get_used(); i+=4)
	{
		ram.allocate(i, disk.read_instruction(i));
	}
	for(int i = 0; i < disk.get_used(); i += 4)
	{
		std::cout << std::hex << ram.get_instruction(i) << std::endl;
	}
}

#include <cstdlib>
#include <string>
#include "../src/loader.hpp"
#include "../src/disk.hpp"

int main()
{
	Disk disk;
	loader load;

    load.readFromFile("Program-File.txt", disk);
}

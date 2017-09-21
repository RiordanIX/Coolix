#pragma once
#include <cstdlib>
#include <fstream>
#include <string>
#include "disk.hpp"

class loader
{
private:
    std::ifstream file;
	Disk _disk;

public:
	loader(Disk disk);
    void readFromFile(std::string filename);
};

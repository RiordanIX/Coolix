#pragma once
#include <cstdlib>
#include <fstream>
#include <string>
#include "debug.hpp"
#include "disk.hpp"

class loader {
private:
	std::ifstream file;

public:
	void readFromFile(std::string filename);
};


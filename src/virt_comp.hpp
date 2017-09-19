#pragma once
#include <iostream>
#include "ram.hpp"
#include "disc.hpp"

/** Virtual Computer
 * Creates the virtual computer using stats from a file.
 * Loads ram size and disc size stats from a file so it can be changed from run
 * to run.
 */
class virt_comp {
public:
	virt_comp();
	int setup(char* argv[]);
private:
	Ram _ram();
	Disc _disc();
	cpu _cpu();
};


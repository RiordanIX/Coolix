#pragma once
// fstream for reading stat files for virtual machine specifics
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ram.hpp"
#include "disk.hpp"
#include "cpu.hpp"

/** Virtual Computer
 * Creates the virtual computer using stats from a file.
 * Loads ram size and disc size stats from a file so it can be changed from run
 * to run.
 */
class virt_comp {
public:
	virt_comp(unsigned long ram_size, unsigned long disk_size, unsigned long cpus=1);
	void report();

	Ram ram;
	Disk disk;
	unsigned long num_cpus;
	std::vector<cpu> cpus;
};

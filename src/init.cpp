#include <iostream>
#include "memory_constants.hpp"
#include "virt_comp.hpp"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "Usage:  coolix <run_file>" << std::endl;
		// Prevent warning using -Wall.
		std::cout << argc << " " << argv << std::endl;
	}
	else {
		std::ifstream stats("stats.stat");
		if (stats.is_open()) {
			// TODO Do some error checking in case stats.stat is messed up.
			// Also, the stats.stat file needs to end with a newline
			std::string line;
			getline(stats, line);
			RAM_SIZE = std::stoul(line);

			getline(stats, line);
			DISK_SIZE = std::stoul(line);

			getline(stats, line);
			num_cpus = std::stoul(line);
		}
		else {
			RAM_SIZE = 4096;
			DISK_SIZE = 8192;
			num_cpus = 1;
		}
		stats.close();

		virt_comp computer = virt_comp(RAM_SIZE, DISK_SIZE);
		computer.report();
	}
}


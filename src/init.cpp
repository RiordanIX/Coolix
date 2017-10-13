#include <iostream>
#include "virt_comp.hpp"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "Usage:  coolix <run_file>" << std::endl;
		// Prevent warning using -Wall.
		std::cout << argc << " " << argv << std::endl;
	}
	else {
		unsigned long ram_size, disk_size;
		std::ifstream stats("stats.stat");
		if (stats.is_open()) {
			// TODO Do some error checking in case stats.stat is messed up.
			// Also, the stats.stat file needs to end with a newline
			std::string line;
			getline(stats, line);
			ram_size = std::stoul(line);

			getline(stats, line);
			disk_size = std::stoul(line);

			getline(stats, line);
			num_cpus = std::stoul(line);
		}
		else {
			ram_size = 4096;
			disk_size = 8192;
			num_cpus = 1;
		}
		stats.close();

		virt_comp computer = virt_comp(ram_size, disk_size);
		computer.report();
	}
}


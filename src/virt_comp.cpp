#include "virt_comp.hpp"

virt_comp::virt_comp() {
	std::ifstream stats("stats.stat");
	if (stats.is_open()) {
		// TODO Do some error checking in case stats.stat is messed up.
		// Also, the stats.stat file needs to end with a newline
		string line;

		getline(stats, line);
		_ram(std::stoul(line));

		getline(stats, line);
		_disk(std::stoul(line));
		stats.close();
	}
	else { // Use default information
		_ram(DEFAULT_RAM);
		_disk(DEFAULT_DISK);
	}

	_cpu();
}


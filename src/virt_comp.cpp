#include "virt_comp.hpp"

virt_comp::virt_comp() {
	std::ifstream stats("stats.stat");
	if (stats.is_open()) {
		// TODO Do some error checking in case stats.stat is messed up.
		_ram(unsigned int(stats << '\n'));
		_disk(unsigned int(stats << '\n'));
	}
	else { // Use default information
		_ram(DEFAULT_RAM);
		_disk(DEFAULT_DISK);
	}
	_cpu();
}


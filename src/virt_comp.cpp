#include "virt_comp.hpp"


virt_comp::virt_comp() {
	std::ifstream stats("stats.stat");
	if (stats.is_open()) {
		// TODO Do some error checking in case stats.stat is messed up.
		// Also, the stats.stat file needs to end with a newline
		std::string line;

		getline(stats, line);
		_ram = Ram(std::stoul(line));

		getline(stats, line);
		_disk = Disk(std::stoul(line));
		_cpu = cpu();

		stats.close();
		// returning because can not have an else due to compiler warnings
		return;
	}
	// No else here due to compiler warnings.
	_ram = Ram();
	_disk = Disk();
	_cpu = cpu();
}


/** Prints information regarding the state of the virtual machine
 *
 */
void virt_comp::report() {
	std::cout << _ram.get_info() << std::endl;
	std::cout << _disk.get_info() << std::endl;
	std::cout << _cpu.get_info() << std::endl;
}


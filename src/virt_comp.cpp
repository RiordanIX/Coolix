#include "virt_comp.hpp"


virt_comp::virt_comp(unsigned long ram_size, unsigned long disk_size,
					unsigned long num_cpus) :
		ram(ram_size), disk(disk_size), num_cpus(num_cpus),
		cpus(_num_cpus, cpu()) {}


/** Prints information regarding the state of the virtual machine
 *
 */
void virt_comp::report() {
	std::cout << ram.get_info() << std::endl;
	std::cout << disk.get_info() << std::endl;
	for (int i = 0; i < cpus.size(); ++i) {
		std::cout << "CPU: " << i << std::endl
		          << cpus[i].get_info() << std::endl;
	}
}


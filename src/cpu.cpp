#include "cpu.hpp"

void cpu::execute(instruct_t inst) {
	// TODO Actually execute something
	std::cout << inst << std::endl;
}


std::string cpu::get_info() {
	std::string info = "CPU Registers:\n";
	for (auto &i : registers) {
		info += std::to_string(i) + '\n';
	}
	return info;
}


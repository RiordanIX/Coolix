#include "cpu.hpp"

void cpu::decode(instruct_t inst) {
	std::cout << inst << std::endl;
	instruct_t format_code = inst & FORMAT_CODE_MASK;
	if (format_code == INST_ARITHMETIC) {
		cpu_arithmetic_operation(inst);
	}
	else if (format_code == INST_CONDTL_AND_IMMEDIATE) {
		cpu_condtl_and_immediate_operation(inst);
	}
	else if (format_code == INST_UNCONDITIONAL) {
		cpu_unconditional_operation(inst);
	}
	else if (format_code == INST_IO) {
		cpu_io_operation(inst);
	}
	else {
		std::cout << get_info() << std::endl;
		throw "Error while decoding instruction";
	}
}


std::string cpu::get_info() {
	std::string info = "CPU Registers:\n";
	for (auto &i : registers) {
		info += std::to_string(i) + '\n';
	}
	return info;
}



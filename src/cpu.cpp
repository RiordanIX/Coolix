#include "cpu.hpp"
#include "cpu_operations.hpp"

void cpu::decode_and_execute(instruct_t inst, PCB* pcb) {
	std::cout << inst << std::endl;
	instruct_t format_code = inst & FORMAT_CODE_MASK;
	instruct_t opcode = inst & OPCODE_MASK;
	opcode = opcode >> (3*8); // 3 bytes to get to lowest position

	if (format_code == INST_ARITHMETIC) {
		cpu_arithmetic_operation(inst, opcode, pcb);
	}
	else if (format_code == INST_CONDTL_AND_IMMEDIATE) {
		cpu_immediate_operation(inst, opcode, pcb);
	}
	else if (format_code == INST_UNCONDITIONAL) {
		cpu_unconditional_operation(inst, opcode, pcb);
	}
	else if (format_code == INST_IO) {
		cpu_io_operation(inst, opcode, pcb);
	}
	else {
		std::cout << get_info() << std::endl;
		throw "Error while decoding instruction";
	}
}

instruct_t cpu::fetch(PCB* pcb) {
	return MEM[pcb->PC + pcb->get_ram_address()];
}

std::string cpu::get_info() {
	std::string info = "CPU Registers:\n";
	for (auto &i : registers) {
		info += std::to_string(i) + '\n';
	}
	return info;
}


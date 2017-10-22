#include "cpu_defs.hpp"
#include "cpu.hpp"

#ifdef DEBUG
#include <cstdio>
#endif

extern Ram MEM;

void cpu::decode_and_execute(instruct_t inst, PCB* pcb) {
#ifdef DEBUG
	printf("This Instruction: %#010X\n",inst);
#endif //DEBUG

	instruct_t format_code = inst & FORMAT_CODE_MASK;
	instruct_t opcode = inst & OPCODE_MASK;
	opcode = opcode >> (3*8); // 3 bytes to get to lowest position

	if (format_code == INST_ARITHMETIC) {
		cpu_arithmetic_operation(inst, opcode);
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
#ifdef DEBUG
	std::cout << get_info();
#endif
}

instruct_t cpu::fetch(PCB* pcb) {
	return MEM.get_instruction(pcb->get_program_counter() + pcb->get_ram_address());
}

std::string cpu::get_info() {
	std::string info = "CPU Registers:\n";
	for (auto &i : registers) {
		info += std::to_string(i) + '\n';
	}
	return info;
}

void cpu::set_registers(std::vector<instruct_t> source) {
	int i = 0;
	for (auto it = source.begin(); it != source.end(); it++){
		registers[i] = *it;
		i++;
	}
}


void cpu::cpu_rd(instruct_t Reg1, instruct_t Reg2, instruct_t Address, instruct_t offset) {
	//registers[Reg1] = (Address == 0) ? registers[Reg2] : MEM.get_instruction(Address + offset);
	if (Address == 0) {
		registers[Reg1] = MEM.get_instruction(registers[Reg2]+offset);
	}
	else {
		registers[Reg1] = MEM.get_instruction(Address + offset);
		}
}


void cpu::cpu_wr(instruct_t Reg1, instruct_t Reg2, instruct_t Address, instruct_t offset) {
	if(Address == 0)
		registers[Reg2] = registers[Reg1];
	else
		#ifdef DEBUG
		{
		printf("Writing: %#010X (%d in decimal), write location: %#010X\n", registers[Reg1], registers[Reg1], Address);
		printf("End address: %#010X\n", offset);
		MEM.allocate(Address + offset, registers[Reg1]);
		}
		#else
		MEM.allocate(Address + offset, registers[Reg1]);
		#endif
}


void cpu::cpu_st(instruct_t B_reg, instruct_t D_reg, instruct_t Address, instruct_t offset) {
	if (B_reg == 0) {
		registers[D_reg] = Address;
	}
	else {
		// Address acts as an offset for the Base register
		instruct_t temp = MEM.get_instruction(offset + registers[D_reg]);
		MEM.allocate(offset + registers[B_reg], temp);
	}
}


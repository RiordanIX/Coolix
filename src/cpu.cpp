#include "cpu_defs.hpp"
#include "cpu.hpp"
#include <mutex>
#include <thread>

#ifdef DEBUG
#include <cstdio>
#endif

extern Ram MEM;
std::mutex mtx;

void cpu::decode_and_execute(instruct_t inst, PCB* pcb) {
	printf("This Instruction: %#010X\n", inst);

	instruct_t format_code = inst & FORMAT_CODE_MASK;
	instruct_t opcode = inst & OPCODE_MASK;
	opcode = opcode >> (3 * 8); // 3 bytes to get to lowest position

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
	//printf("%s", get_info().c_str());
}

instruct_t fetch(cpu *CPU, PCB* pcb) {
	std::lock_guard<std::mutex> lock(mtx);
	if (CPU->cache.in_cache(pcb->get_pid(), pcb->get_program_counter() / (4))){
		pcb->cache_hit();
	}
	else {
		pcb->cache_miss();
		// Lock MEMORY access
		instruct_t inst = 0;
		for (size_t i = 0; i < CPU->cache.size(); i ++) {
			inst = MEM.get_instruction(pcb->get_program_counter() +
							pcb->get_ram_address() + (i * 4));
			CPU->cache.set_cache(pcb->get_program_counter() + (i *4), inst, i);
			}
	}
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
	for (auto it = source.begin(); it != source.end(); it++) {
		registers[i] = *it;
		i++;
	}
}


void cpu_rd(cpu *CPU, PCB* pcb, instruct_t Reg1, instruct_t Reg2, instruct_t Address, instruct_t offset) {
	//registers[Reg1] = (Address == 0) ? registers[Reg2] : MEM.get_instruction(Address + offset);
	std::lock_guard<std::mutex> lock(mtx);
	if (Reg2 > 0) {
		if (CPU->cache.in_cache(pcb->get_pid(), CPU->registers[Reg2]/(4))){
			pcb->cache_hit();
			CPU->registers[Reg1] = CPU->cache.get_instruction(CPU->registers[Reg2]/(4));
		}
		else {
			pcb->cache_miss();
			// Lock MEMORY
			CPU->registers[Reg1] = MEM.get_instruction(CPU->registers[Reg2] + offset);
		}
	}
	else {
		if (CPU->cache.in_cache(pcb->get_pid(), Address / (4))){
			pcb->cache_hit();
			CPU->registers[Reg1] = CPU->cache.get_instruction(Address / (4));
		}
		else {
			pcb->cache_miss();
			// Lock Memory
			for (size_t i = 0; i < CPU->cache.size(); i ++) {
				instruct_t inst = MEM.get_instruction(pcb->get_program_counter() +
								pcb->get_ram_address() + (i * 4));
				CPU->cache.set_cache(pcb->get_program_counter() + (i *4), inst, i);
			}
			CPU->registers[Reg1] = MEM.get_instruction(Address + offset);
		}
	}
}


void cpu_wr(cpu* CPU, instruct_t Reg1, instruct_t Reg2, instruct_t Address, instruct_t offset) {
	//mtx.lock();
	if (Reg2 > 0)
		CPU->registers[Reg2] = CPU->registers[Reg1];
	else {
	printf("Writing: %#010X (%d in decimal), write location: %#010X\n",
			CPU->registers[Reg1], CPU->registers[Reg1], Address);
		printf("End address: %#010X\n", offset);
		// Lock Memory
		std::lock_guard<std::mutex> lock(mtx);
		MEM.allocate(Address + offset, CPU->registers[Reg1]);
	}
	//mtx.unlock();
}


inline void cpu::cpu_mov(instruct_t s1, instruct_t dest) {
	registers[dest] = registers[s1];
}

inline void cpu::cpu_add(instruct_t s1, instruct_t s2, instruct_t dest) {
	registers[dest] = registers[s1] + registers[s2];
}

inline void cpu::cpu_sub(instruct_t s1, instruct_t s2, instruct_t dest) {
	registers[dest] = registers[s1] - registers[s2];
}

inline void cpu::cpu_mul(instruct_t s1, instruct_t s2, instruct_t dest) {
	registers[dest] = registers[s1] * registers[s2];
}

inline void cpu::cpu_div(instruct_t s1, instruct_t s2, instruct_t dest) {
	registers[dest] = registers[s1] / registers[s2];
}

// Logical AND
inline void cpu::cpu_and(instruct_t s1, instruct_t s2, instruct_t dest) {
	registers[dest] = registers[s1] & registers[s2];
}

// Logical OR
inline void cpu::cpu_or(instruct_t s1, instruct_t s2, instruct_t dest) {
	registers[dest] = registers[s1] | registers[s2];
}

// Set value to 1 if s1 < s2, 0 otherwise
inline void cpu::cpu_slt(instruct_t s1, instruct_t s2, instruct_t dest) {
	registers[dest] = registers[s1] < registers[s2] ? 1 : 0;
}

void cpu_st(cpu* CPU, instruct_t B_reg, instruct_t D_reg, instruct_t offset) {
	//mtx.lock();
	std::lock_guard<std::mutex> lock(mtx);
	MEM.allocate(offset + CPU->registers[D_reg], CPU->registers[B_reg]);
	//mtx.unlock();
}

void cpu_lw(cpu* CPU, instruct_t B_reg, instruct_t D_reg, instruct_t Address, instruct_t offset) {
	//mtx.lock();
	std::lock_guard<std::mutex> lock(mtx);
	CPU->registers[D_reg] = MEM.get_instruction( CPU->registers[B_reg] + Address + offset);
	//mtx.unlock();
}

inline void	cpu::cpu_movi(instruct_t D_reg, instruct_t Address) {
	registers[D_reg] = Address;
}

inline void	cpu::cpu_addi(instruct_t B_reg, instruct_t D_reg, instruct_t Address) {
	if (B_reg == 0)
		registers[D_reg] += Address;
}

inline void	cpu::cpu_muli(instruct_t B_reg, instruct_t D_reg, instruct_t Address) {
	if (B_reg == 0)
		registers[D_reg] *= Address;
}

inline void	cpu::cpu_divi(instruct_t D_reg, instruct_t Address) {
	registers[D_reg] /= Address;
}

inline void	cpu::cpu_ldi(instruct_t D_reg, instruct_t Address) {
	registers[D_reg] = Address;
}

inline void	cpu::cpu_slti(instruct_t B_reg, instruct_t D_reg, instruct_t Address) {
	registers[D_reg] = registers[B_reg] < Address ? 1 : 0;
}

inline void	cpu::cpu_hlt(PCB* pcb) {
	pcb->set_status(status::TERMINATED);
	for (int i = 0; i < 16; i++)
		registers[i] = 0;
}

inline void	cpu::cpu_jmp(instruct_t Address,/* instruct_t offset,*/PCB* pcb) {
	pcb->set_program_counter(Address - 4);
}

inline void	cpu::cpu_beq(instruct_t B_reg, instruct_t D_reg, instruct_t Address, PCB* pcb) {
	if (registers[B_reg] == registers[D_reg])
		pcb->set_program_counter(Address - 4);
}

inline void	cpu::cpu_bne(instruct_t B_reg, instruct_t D_reg, instruct_t Address, PCB* pcb) {
	if (registers[B_reg] != registers[D_reg])
		pcb->set_program_counter(Address - 4);
}

inline void	cpu::cpu_bez(instruct_t B_reg, instruct_t Address, PCB* pcb) {
	if (registers[B_reg] == 0)
		pcb->set_program_counter(Address - 4);
}

inline void	cpu::cpu_bnz(instruct_t B_reg, instruct_t Address, PCB* pcb) {
	if (registers[B_reg] != 0)
		pcb->set_program_counter(Address - 4);
}

inline void	cpu::cpu_bgz(instruct_t B_reg, instruct_t Address, PCB* pcb) {
	if (!(registers[B_reg] & 0x80000000))
		pcb->set_program_counter(Address - 4);
}

inline void	cpu::cpu_blz(instruct_t B_reg, instruct_t Address, PCB* pcb) {
	if (registers[B_reg] & 0x80000000)
		pcb->set_program_counter(Address - 4);
}

inline void cpu::cpu_io_operation(instruct_t inst, instruct_t opcode, PCB* pcb) {
	instruct_t Reg1, Reg2, Address, offset;
	Reg1 = (inst & 0x00F00000) >> (5 * 4);
	Reg2 = (inst & 0x000F0000) >> (4 * 4);
	Address = inst & 0x0000FFFF;
	offset = pcb->get_ram_address();

	switch (opcode)
	{
	case OP_IO_RD:
		cpu_rd(this, pcb,Reg1, Reg2, Address, offset);
		break;

	case OP_IO_WR:
		cpu_wr(this, Reg1, Reg2, Address, offset);
		break;
	default:
		throw "Invalid IO instruction format";
	}
}

inline void cpu::cpu_immediate_operation(instruct_t inst, instruct_t opcode, PCB* pcb) {
	instruct_t B_reg, D_reg, Address;
	B_reg = (inst & 0x00F00000) >> (5 * 4);
	D_reg = (inst & 0x000F0000) >> (4 * 4);
	Address = inst & 0x0000FFFF;

	switch (opcode) {
	case OP_I_ST:
		cpu_st(this, B_reg, D_reg,pcb->get_ram_address());
		break;
	case OP_I_LW:
		cpu_lw(this, B_reg, D_reg, Address,pcb->get_ram_address());
		break;
	case OP_I_MOVI:
		cpu_movi(D_reg, Address);
		break;
	case OP_I_ADDI:
		cpu_addi(B_reg, D_reg, Address);
		break;
	case OP_I_MULI:
		cpu_muli(B_reg, D_reg, Address);
		break;
	case OP_I_DIVI:
		cpu_divi(D_reg, Address);
		break;
	case OP_I_LDI:
		cpu_ldi(D_reg, Address);
		break;
	case OP_I_SLTI:
		cpu_slti(B_reg, D_reg, Address);
		break;
	case OP_I_BEQ:
		cpu_beq(B_reg, D_reg, Address, pcb);
		break;
	case OP_I_BNE:
		cpu_bne(B_reg, D_reg, Address, pcb);
		break;
	case OP_I_BEZ:
		cpu_bez(B_reg, Address, pcb);
		break;
	case OP_I_BNZ:
		cpu_bnz(B_reg, Address, pcb);
		break;
	case OP_I_BGZ:
		cpu_bgz(B_reg, Address, pcb);
		break;
	case OP_I_BLZ:
		cpu_blz(B_reg, Address, pcb);
		break;
	default:
		throw "Invalid Immediate instruction format";
	}
}


cpu CPU0, CPU1, CPU2, CPU3;


#include "cpu_defs.hpp"
#include "cpu.hpp"
#include "mmu.hpp"

#ifdef DEBUG
#include <cstdio>
#endif

using std::size_t;
extern mmu MMU;

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

instruct_t cpu::fetch(PCB* pcb)
{
	size_t frame = pcb->get_frame(pcb->get_program_counter() / (PAGE_SIZE));
	size_t offset = (pcb->get_program_counter() % (PAGE_SIZE));
	// Cache hit
	if (cache.in_cache(pcb->get_pid(), frame)) {
		debug_printf("Frame is in the cache%s","\n");
		return cache.get_instruction(frame, offset);
	}
	// Cache Miss
	else if (pcb->is_valid_page(pcb->get_program_counter() / (PAGE_SIZE)))
	{
		if ((offset + pcb->get_program_counter()) / (INST_SIZE) <= 3)
		{
			std::vector<instruct_t> insts = MMU.get_frame_data(pcb);
			debug_printf("Setting the cache%s", "\n");
			cache.set_cache(frame, insts);
		}
		return MMU.get_instruction(pcb);
	}
	else
	{
		pcb->set_lastRequestedPage(pcb->get_program_counter() / (PAGE_SIZE));
		pcb->set_waitformmu(true);
		pcb->set_status(WAITING); //fetch method
		return -1;
	}
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


inline void cpu::cpu_rd(PCB* pcb, instruct_t Reg1, instruct_t Reg2, instruct_t Address) {

		size_t pageNumber = (Address / (PAGE_SIZE));
		if (Reg2 > 0)
		{
			//Address = (registers[Reg2] / (PAGE_SIZE));
			//offset = (registers[Reg2] % (PAGE_SIZE));
			pageNumber = (registers[Reg2] / (PAGE_SIZE));
			if (pcb->is_valid_page(pageNumber) )
			{
				registers[Reg1] = MMU.get_instruction(pcb, registers[Reg2]);
			}
			else
			{
				//page fault
				pcb->set_lastRequestedPage(pageNumber);
				pcb->set_waitformmu(true);
				pcb->set_status(WAITING);
			}
		}
		else {
			if (pcb->is_valid_page(pageNumber))
			{
				registers[Reg1] = MMU.get_instruction(pcb, Address);
			}
			else
			{
				//page fault
				pcb->set_lastRequestedPage(pageNumber);
				pcb->set_waitformmu(true);
				pcb->set_status(WAITING);
			}
		}
}


inline void cpu::cpu_wr(PCB* pcb, instruct_t Reg1, instruct_t Reg2, instruct_t Address, instruct_t offset) {
	if (Reg2 > 0)
		registers[Reg2] = registers[Reg1];
	else {

		printf("Writing: %#010X (%d in decimal), write location: %#010X\n",
			registers[Reg1], registers[Reg1], Address);
		printf("End address: %#010X\n", offset);
		// Needs some sort of offset.  This should be figured out some way.
		if (pcb->is_valid_page(Address / (PAGE_SIZE)))
		{
			size_t frame = pcb->get_frame(Address / (PAGE_SIZE));
			offset = Address % (PAGE_SIZE);
			MMU.writeToRam(frame, offset, registers[Reg1]);
		}
		else
		{
			//page fault
			pcb->set_lastRequestedPage(Address / (PAGE_SIZE));
			pcb->set_waitformmu(true);
			pcb->set_status(WAITING);
		}
	}
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

inline void cpu::cpu_st(instruct_t B_reg, instruct_t D_reg, PCB * pcb) {

	if (pcb->is_valid_page(registers[D_reg] / (PAGE_SIZE) ))
	{
		std::size_t frame = pcb->get_frame((registers[D_reg]) / (PAGE_SIZE));
		instruct_t offset = (registers[D_reg]) % (PAGE_SIZE);
	//	MMU.writeToRam(offset + registers[D_reg], registers[B_reg]);
		MMU.writeToRam(frame,offset, registers[B_reg]);
	}
	else
	{
		//page fault
		pcb->set_lastRequestedPage(registers[D_reg] / (PAGE_SIZE));
		pcb->set_waitformmu(true);
		pcb->set_status(WAITING);
	}

}

inline void	cpu::cpu_lw(PCB* pcb, instruct_t B_reg, instruct_t D_reg, instruct_t Address) {

	if (pcb->is_valid_page((registers[B_reg] + Address) / (PAGE_SIZE)))
	{
	//	Address = (registers[B_reg] + Address) / (PAGE_SIZE);
	//	instruct_t offset = (registers[B_reg] + Address) % (PAGE_SIZE);
		registers[D_reg] = MMU.get_instruction(pcb, registers[B_reg] + Address);
	}
	else
	{
		//page fault
		pcb->set_lastRequestedPage(registers[D_reg] / (PAGE_SIZE));
		pcb->set_waitformmu(true);
		pcb->set_status(WAITING);
	}
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
	//for (int i = 0; i < 16; i++)
		//registers[i] = 0;
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

	//size_t frameNum = Address / (PAGE_SIZE);
	offset = Address % (PAGE_SIZE);

	switch (opcode)
	{
	case OP_IO_RD:

		cpu_rd(pcb, Reg1, Reg2, Address);
		break;

	case OP_IO_WR:
		cpu_wr(pcb, Reg1, Reg2, Address, offset);
		break;
	default:
		throw "Invalid IO instruction format";
	}
}

// GLOBAL VARIABLES
cpu CPU0, CPU1, CPU2, CPU3;


#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include "instruct.hpp"
#include "pcb.hpp"
#include "ram.hpp"
#include "cpu_defs.hpp"

using std::vector;
using std::size_t;
// Default Register size is 16.  May change if requirements change.
#define DEFAULT_REG_SIZE 16


/** cpu
 * A simulated CPU class. Simulates some aspects of a normal CPU.  The user
 * can define the number of registers by passing it in, or just leaving it as
 * the default of 16.
 */
class cpu {
public:
	// Public because the dispatcher needs to access it.
	size_t num_registers;
	vector<instruct_t> registers;

	cpu(size_t size=DEFAULT_REG_SIZE) : num_registers(size), registers(size, 0) {}
	instruct_t fetch(PCB* pcb);
	void decode_and_execute(instruct_t inst, PCB* pcb);
	std::string get_info();


/******************************************************************************
 * Emulated cpu operations
 * The following functions are used to emulate the operations of the cpu
 * itself.  They should all defined as inline, and they should only have a
 * couple lines at most.
 *****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
//  I/O instructions {{{
///////////////////////////////////////////////////////////////////////////////
inline void cpu_rd(instruct_t Reg1, instruct_t Reg2, instruct_t Address, instruct_t offset) {
	registers[Reg1] = (Address == 0) ? registers[Reg2] : MEM.get_instruction(Address + offset);
}

inline void cpu_wr(instruct_t Reg1, instruct_t Reg2, instruct_t Address, instruct_t offset) {
	if(Address == 0)
		registers[Reg2] = registers[Reg1];
	else
		MEM.allocate(Address + offset, registers[Reg1]);
}
// }}}


///////////////////////////////////////////////////////////////////////////////
//  Arithmetic instructions {{{
///////////////////////////////////////////////////////////////////////////////
inline void cpu_mov(instruct_t s1, instruct_t dest ) {
	registers[dest] = registers[s1];
}

inline void cpu_add(instruct_t s1, instruct_t s2, instruct_t dest ) {
	registers[dest] = registers[s1] + registers[s2];
}

inline void cpu_sub(instruct_t s1, instruct_t s2, instruct_t dest ) {
	registers[dest] = registers[s1] - registers[s2];
}

inline void cpu_mul(instruct_t s1, instruct_t s2, instruct_t dest ) {
	registers[dest] = registers[s1] * registers[s2];
}

inline void cpu_div(instruct_t s1, instruct_t s2, instruct_t dest ) {
	registers[dest] = registers[s1] / registers[s2];
}

// Logical AND
inline void cpu_and(instruct_t s1, instruct_t s2, instruct_t dest ) {
	registers[dest] = registers[s1] && registers[s2];
}

// Logical OR
inline void cpu_or(instruct_t s1, instruct_t s2, instruct_t dest ) {
	registers[dest] = registers[s1] || registers[s2];
}

// Sets the D-reg to 1 if first S-reg is less than the B-reg; 0 otherwise
// Different than the other Arithmetic instructions, s1 is the register that
// gets changed.
inline void cpu_slt(instruct_t s1, instruct_t s2, instruct_t dest ) {
	registers[s1] = registers[s2] < registers[dest] ? 1 : 0;
}

// }}}

///////////////////////////////////////////////////////////////////////////////
// Immediate instructions {{{
// When the last 16 bits contain data, the D-reg is always 0000
///////////////////////////////////////////////////////////////////////////////
inline void	cpu_st(instruct_t B_reg, instruct_t D_reg, instruct_t Address, instruct_t offset) {
	if (B_reg == 0) {
		registers[D_reg] = Address;
	}
	else {
		// Address acts as an offset for the Base register
		registers[D_reg] = MEM.get_instruction(registers[B_reg] + Address + offset);
	}
}

inline void	cpu_lw(instruct_t B_reg, instruct_t D_reg) {
	registers[D_reg] = registers[B_reg];
}

inline void	cpu_movi(instruct_t D_reg, instruct_t Address) {
	registers[D_reg] = Address;
}
inline void	cpu_addi(instruct_t B_reg, instruct_t D_reg, instruct_t Address) {
	if (registers[B_reg] == 0) {
		registers[D_reg] += Address;
	}
}

inline void	cpu_muli(instruct_t B_reg, instruct_t D_reg, instruct_t Address) {
	if (registers[B_reg] == 0) {
		registers[D_reg] *= Address;
	}
}

inline void	cpu_divi(instruct_t D_reg, instruct_t Address) {
	registers[D_reg] /= Address;
}

inline void	cpu_ldi(instruct_t D_reg, instruct_t Address) {
	registers[D_reg] = Address;
}

inline void	cpu_slti(instruct_t B_reg, instruct_t D_reg, instruct_t Address) {
	registers[D_reg] = registers[B_reg] < Address ? 1 : 0;
}

inline void	cpu_hlt(PCB* pcb) {
	pcb->set_status(status::TERMINATED);
}

inline void	cpu_jmp(instruct_t Address,/* instruct_t offset,*/PCB* pcb) {
	pcb->set_program_counter(Address);
}

inline void	cpu_beq(instruct_t B_reg, instruct_t D_reg, instruct_t Address, PCB* pcb) {
	if (registers[B_reg] == registers[D_reg])
		pcb->set_program_counter(Address);

}

inline void	cpu_bne(instruct_t B_reg, instruct_t D_reg, instruct_t Address, PCB* pcb) {
	if (registers[B_reg] != registers[D_reg])
		pcb->set_program_counter(Address);
}

inline void	cpu_bez(instruct_t B_reg, instruct_t Address, PCB* pcb) {
	if (registers[B_reg] == 0)
		pcb->set_program_counter(Address);
}

inline void	cpu_bnz(instruct_t B_reg, instruct_t Address, PCB* pcb) {
	if (registers[B_reg] != 0)
		pcb->set_program_counter(Address);
}

inline void	cpu_bgz(instruct_t B_reg, instruct_t Address, PCB* pcb) {
	if (!(registers[B_reg] & 0x80000000))
		pcb->set_program_counter(Address);
}
inline void	cpu_blz(instruct_t B_reg, instruct_t Address, PCB* pcb) {
	if (registers[B_reg] & 0x80000000)
		pcb->set_program_counter(Address);
}

// End of Immediate instructions
// }}}


/** Arithmetic Operations {{{
 * Determines which operation to use.
 * Essentially a glorified switch statement.  Separated out so easier to read.
 * Arithmetic operations all have the _R_ part in the opcode definition
 */
inline void cpu_arithmetic_operation(instruct_t inst, instruct_t opcode) {
	instruct_t S1, S2, Dest;
	// Each hex position is worth 4 bits, so 5 Hexes = 5*4 bits
	S1   = (inst & 0x00F00000) >> (5*4);
	S2   = (inst & 0x000F0000) >> (4*4);
	Dest = (inst & 0x0000F000) >> (3*4);
	switch (opcode){
		case OP_R_MOV:
			cpu_mov(S1, Dest);
			break;
		case OP_R_ADD:
			cpu_add(S1, S2, Dest);
			break;
		case OP_R_SUB:
			cpu_sub(S1, S2, Dest);
			break;
		case OP_R_MUL:
			cpu_mul(S1, S2, Dest);
			break;
		case OP_R_DIV:
			cpu_div(S1, S2, Dest);
			break;
		case OP_R_AND:
			cpu_and(S1, S2, Dest);
			break;
		case OP_R_OR:
			cpu_or(S1, S2, Dest);
			break;
		case OP_R_SLT:
			cpu_slt(S1, S2, Dest);
			break;
		default:
			throw "Improper arithmetic operation.";
	}
}
// }}} End of Arithmetic Operations


// Immediate Operations {{{
inline void cpu_immediate_operation(instruct_t inst, instruct_t opcode, PCB* pcb) {
	instruct_t B_reg, D_reg, Address, offset;
	B_reg   = (inst & 0x00F00000) >> (5*4);
	D_reg   = (inst & 0x000F0000) >> (4*4);
	Address =  inst & 0x0000FFFF;
	offset  = pcb->get_ram_address();

	switch (opcode) {
		case OP_I_ST:
			cpu_st(B_reg, D_reg, Address, offset);
			break;
		case OP_I_LW:
			cpu_lw(B_reg, D_reg);
			break;
		case OP_I_MOVI:
			cpu_movi( D_reg, Address);
			break;
		case OP_I_ADDI:
			cpu_addi(B_reg, D_reg, Address);
			break;
		case OP_I_MULI:
			cpu_muli(B_reg, D_reg, Address);
			break;
		case OP_I_DIVI:
			cpu_divi( D_reg, Address);
			break;
		case OP_I_LDI:
			cpu_ldi( D_reg, Address);
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
// }}} End of Immediate Operations

// Unconditional Operations {{{
inline void cpu_unconditional_operation(instruct_t inst, instruct_t opcode, PCB* pcb) {
	instruct_t Address;
	Address =  inst & 0x00FFFFFF;
	//offset  = pcb->get_ram_address();

	switch(opcode)
	{
		case OP_J_JMP:
			cpu_jmp(Address,/* offset,*/ pcb);
			break;

		case OP_J_HLT:
			cpu_hlt(pcb);
			break;
		default:
			throw "Invalid Unconditional Operation Format";
	}
}
// }}}  End of Unconditional Operations


// IO Operations{{{
inline void cpu_io_operation(instruct_t inst, instruct_t opcode, PCB* pcb) {
	instruct_t Reg1, Reg2, Address, offset;
	Reg1 = (inst & 0x00F00000) >> (5*4);
	Reg2 = (inst & 0x000F0000) >> (4*4);
	Address = inst & 0x0000FFFF;
	offset = pcb->get_ram_address();

	switch(opcode)
	{
		case OP_IO_RD:
			cpu_rd(Reg1, Reg2, Address, offset);
			break;

		case OP_IO_WR:
			cpu_wr(Reg1, Reg2, Address, offset);
			break;
		default:
			throw "Invalid IO instruction format";
	}
}

// }}} End of IO operations

};


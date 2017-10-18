#pragma once
#include "instruct.hpp"
#include "pcb.hpp"


/*****************************************************************************
 * Masks {{{
 * These define certain masks used in interpreting the instruction calls.
 *****************************************************************************/
#define FORMAT_CODE_MASK   0xC0000000
#define OPCODE_MASK        0x3F000000

#define REG_ARITHMETIC_S1  0x00F00000
#define REG_ARITHMETIC_S2  0x000F0000
#define REG_ARITHMETIC_D   0x0000F000

#define REG_CONDITIONAL_B  0x00F00000
#define REG_CONDITIONAL_D  0x000F0000
#define REG_IMMEDIATE_ADDR 0x0000FFFF

#define UNCONDITIONAL_ADDR 0x00FFFFFF

#define IO_REG_1           0x00F00000
#define IO_REG_2           0x000F0000
#define IO_ADDRESS         0x0000FFFF
//}}}


/******************************************************************************
 * Definitions {{{
 * Allow us to more easily know which type of Opcode will be used.
 *****************************************************************************/
#define INST_ARITHMETIC           0x00000000
#define INST_CONDTL_AND_IMMEDIATE 0x40000000
#define INST_UNCONDITIONAL        0x80000000
#define INST_IO                   0xC0000000
//}}}


/******************************************************************************
 * Opcode definitions {{{
 * Below are all the opcodes defined for use in the cpu.
 *****************************************************************************/
#define OP_IO_RD  0x00
#define OP_IO_WR  0x01
#define OP_I_ST   0x02
#define OP_I_LW   0x03
#define OP_R_MOV  0x04
#define OP_R_ADD  0x05
#define OP_R_SUB  0x06
#define OP_R_MUL  0x07
#define OP_R_DIV  0x08
#define OP_R_AND  0x09
#define OP_R_OR   0x0A
#define OP_I_MOVI 0x0B
#define OP_I_ADDI 0x0C
#define OP_I_MULI 0x0D
#define OP_I_DIVI 0x0E
#define OP_I_LDI  0x0F
#define OP_R_SLT  0x10
#define OP_I_SLTI 0x11
#define OP_J_HLT  0x12
#define OP_NOP    0x13
#define OP_I_JMP  0x14
#define OP_I_BEQ  0x15
#define OP_I_BNE  0x16
#define OP_I_BEZ  0x17
#define OP_I_BNZ  0x18
#define OP_I_BGZ  0x19
#define OP_I_BLZ  0x1A
// }}}


/******************************************************************************
 * Emulated cpu operations
 * The following functions are used to emulate the operations of the cpu
 * itself.  They should all defined as inline, and they should only have a
 * couple lines at most.
 *****************************************************************************/
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
	if (D_reg == 0) {
		registers[D_reg] = Address;
	}
	else {
		// Address acts as an offset for the Base register
		registers[D_reg] = MMU[B_reg + Address];
	}
}

inline void	cpu_lw(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
		registers[D_reg] = registers[B_reg];
}
inline void	cpu_movi(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
}
inline void	cpu_addi(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
	if (B_reg == 0) {
		registers[D_reg] += Address;
	}

}

inline void	cpu_muli(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
	if (B_reg == 0) {
		registers[D_reg] *= Address;
	}

}
inline void	cpu_divi(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
	registers[D_reg] /= Address;
}
inline void	cpu_ldi(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
	registers[D_reg] = Address;
}
inline void	cpu_slti(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
}
inline void	cpu_hlt(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
}
inline void	cpu_jmp(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
	PCB->programCounter = Address;	//PCB is a placeholder
	
}
inline void	cpu_beq(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
}
inline void	cpu_bne(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
}
inline void	cpu_bez(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
}
inline void	cpu_bnz(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
}
inline void	cpu_bgz(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
}
inline void	cpu_blz(instruct_t B_reg, instruct_t D_reg, instruct_t Address,
					instruct_t offset) {
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
			throw "Improper arithmetic operation."
	}
}
// }}}


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
			cpu_lw(B_reg, D_reg, Address, offset);
			break;
		case OP_I_MOVI:
			cpu_movi(B_reg, D_reg, Address, offset);
			break;
		case OP_I_ADDI:
			cpu_addi(B_reg, D_reg, Address, offset);
			break;
		case OP_I_MULI:
			cpu_muli(B_reg, D_reg, Address, offset);
			break;
		case OP_I_DIVI:
			cpu_divi(B_reg, D_reg, Address, offset);
			break;
		case OP_I_LDI:
			cpu_ldi(B_reg, D_reg, Address, offset);
			break;
		case OP_I_SLTI:
			cpu_slti(B_reg, D_reg, Address, offset);
			break;
		case OP_I_JMP:
			cpu_jmp(B_reg, D_reg, Address, offset, pcb->programCounter);
			break;
		case OP_I_BEQ:
			cpu_beq(B_reg, D_reg, Address, offset);
			break;
		case OP_I_BNE:
			cpu_bne(B_reg, D_reg, Address, offset);
			break;
		case OP_I_BEZ:
			cpu_bez(B_reg, D_reg, Address, offset);
			break;
		case OP_I_BNZ:
			cpu_bnz(B_reg, D_reg, Address, offset);
			break;
		case OP_I_BGZ:
			cpu_bgz(B_reg, D_reg, Address, offset);
			break;
		case OP_I_BLZ:
			cpu_blz(B_reg, D_reg, Address, offset);
			break;
		default:
			throw "Invalid Immediate instruction format";
	}
}

inline void cpu_unconditional_operation(instruct_t instruct, instruct_t opcode, PCB* pcb) {
	instruct_t B_reg, D_reg, Address, offset;
	B_reg   = (inst & 0x00F00000) >> (5*4);
	D_reg   = (inst & 0x000F0000) >> (4*4);
	Address =  inst & 0x0000FFFF;
	offset  = pcb->get_ram_address();

}



inline void cpu_io_operation(instruct_t instruct, instruct_t opcode) {

}





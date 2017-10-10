#pragma once
#include "instruct.hpp"


/******************************************************************************
 * Masks
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





/******************************************************************************
 * Definitions.
 * Allow us to more easily know which type of Opcode will be used.
 *****************************************************************************/
#define INST_ARITHMETIC           0x00000000
#define INST_CONDTL_AND_IMMEDIATE 0x40000000
#define INST_UNCONDITIONAL        0x80000000
#define INST_IO                   0xC0000000





/******************************************************************************
 * Opcode definitions
 * Below are all the opcodes defined for use in the cpu.
 *****************************************************************************/


/** Determines which operation to use.
 * Essentially a glorified switch statement.  Separated out because so easier
 * to read.
 */
inline void cpu_arithmetic_operation(instruct_t inst, instruct_t opcode) {
	switch (opcode){
		case 1: // Something
		default:
			throw "Improper arithmetic operation."
	}
}





/** Emulated cpu operations
 * The following functions are used to emulate the operations of the cpu
 * itself.  They should all defined as inline, and they should only have a
 * couple lines at most.
 */
inline void cpu_add(instruct_t s1, instruct_t s2, instruct_t dest ) {
	registers[dest] = registers[s1] + registers[s2];
}

inline void cpu_sub(instruct_t s1, instruct_t s2, instruct_t dest ) {
	registers[dest] = registers[s1] - registers[s2];
}

inline void cpu_mov(instruct_t s1, instruct_t s2 ) {
	registers[s2] = registers[s1];
}

inline void cpu_lw(instruct_t reg, instruct_t addr) {
	registers[reg] = addr;
}

inline void cpu_

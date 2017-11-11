#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include "instruct.hpp"
#include "pcb.hpp"
#include "ram.hpp"
#include "cpu_defs.hpp"
#include "debug.hpp"

// Default Register size is 16.  May change if requirements change.
#define DEFAULT_REG_SIZE 16


/******************************************************************************
 * cpu
 * A simulated CPU class. Simulates some aspects of a normal CPU.  The user
 * can define the number of registers by passing it in, or just leaving it as
 * the default of 16.
 *****************************************************************************/
class cpu {
public:
	// Public because the dispatcher needs to access it.
	std::size_t num_registers;
	std::vector<instruct_t> registers;

	cpu(std::size_t size=DEFAULT_REG_SIZE) : num_registers(size), registers(size, 0) {}
	instruct_t fetch(PCB* pcb);
	void set_registers(std::vector<instruct_t> to_switch);
	void decode_and_execute(instruct_t inst, PCB* pcb);
	std::string get_info();


	/**************************************************************************
	 * Emulated cpu operations
	 * The following functions are used to emulate the operations of the cpu
	 * itself.  They should all defined as inline, and they should only have a
	 * couple lines at most.
	 *************************************************************************/
	/**************************************************************************
	 *  I/O instructions {{{
	 *************************************************************************/
	void cpu_rd(instruct_t Reg1, instruct_t Reg2, instruct_t Address, instruct_t offset);
	void cpu_wr(instruct_t Reg1, instruct_t Reg2, instruct_t Address, instruct_t offset);
	/**************************************************************************
	 * }}} End I/O instructions
	 *************************************************************************/


	/**************************************************************************
	 *  Arithmetic instructions {{{
	 *************************************************************************/
	void cpu_mov(instruct_t s1, instruct_t dest );
	void cpu_add(instruct_t s1, instruct_t s2, instruct_t dest );
	void cpu_sub(instruct_t s1, instruct_t s2, instruct_t dest );
	void cpu_mul(instruct_t s1, instruct_t s2, instruct_t dest );
	void cpu_div(instruct_t s1, instruct_t s2, instruct_t dest );
	void cpu_and(instruct_t s1, instruct_t s2, instruct_t dest );
	void cpu_or(instruct_t s1, instruct_t s2, instruct_t dest );
	void cpu_slt(instruct_t s1, instruct_t s2, instruct_t dest );
	/**************************************************************************
	 * }}} End Arithmetic instructions
	 *************************************************************************/


	/**************************************************************************
	 * Immediate instructions {{{
	 * When the last 16 bits contain data, the D-reg is always 0000
	 *************************************************************************/
	void cpu_st(instruct_t B_reg, instruct_t D_reg);
	void cpu_lw(instruct_t B_reg, instruct_t D_reg, instruct_t Address);
	void cpu_movi(instruct_t D_reg, instruct_t Address);
	void cpu_addi(instruct_t B_reg, instruct_t D_reg, instruct_t Address);
	void cpu_muli(instruct_t B_reg, instruct_t D_reg, instruct_t Address);
	void cpu_divi(instruct_t D_reg, instruct_t Address);
	void cpu_ldi(instruct_t D_reg, instruct_t Address);
	void cpu_slti(instruct_t B_reg, instruct_t D_reg, instruct_t Address);
	void cpu_hlt(PCB* pcb);
	void cpu_jmp(instruct_t Address,/* instruct_t offset,*/PCB* pcb);
	void cpu_beq(instruct_t B_reg, instruct_t D_reg, instruct_t Address, PCB* pcb);
	void cpu_bne(instruct_t B_reg, instruct_t D_reg, instruct_t Address, PCB* pcb);
	void cpu_bez(instruct_t B_reg, instruct_t Address, PCB* pcb);
	void cpu_bnz(instruct_t B_reg, instruct_t Address, PCB* pcb);
	void cpu_bgz(instruct_t B_reg, instruct_t Address, PCB* pcb);
	void cpu_blz(instruct_t B_reg, instruct_t Address, PCB* pcb);
	/**************************************************************************
	 * }}} End of Immediate instructions
	 *************************************************************************/


	/**************************************************************************
	 * Arithmetic Operations {{{
	 * Determines which operation to use.
	 * Essentially a glorified switch statement.  Separated out so easier to
	 * read. Arithmetic operations all have the _R_ part in the opcode
	 * definition
	 *************************************************************************/
	inline void cpu_arithmetic_operation(instruct_t inst, instruct_t opcode) {
		instruct_t S1, S2, Dest;
		// Each hex position is worth 4 bits, so 5 Hexes = 5*4 bits
		S1   = (inst & 0x00F00000) >> (5*4);
		S2   = (inst & 0x000F0000) >> (4*4);
		Dest = (inst & 0x0000F000) >> (3*4);
		switch (opcode){
			case OP_R_MOV:
				cpu_mov(S2, Dest);
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
				throw "Illegal arithmetic operation: " + std::to_string(inst);
		}
	}
	/**************************************************************************
	 * }}} End of Arithmetic Operations
	 *************************************************************************/


	/**************************************************************************
	 * Immediate Operations {{{
	 *************************************************************************/
	inline void cpu_immediate_operation(instruct_t inst, instruct_t opcode, PCB* pcb) {
		instruct_t B_reg, D_reg, Address;
		B_reg   = (inst & 0x00F00000) >> (5*4);
		D_reg   = (inst & 0x000F0000) >> (4*4);
		Address =  inst & 0x0000FFFF;

		switch (opcode) {
			case OP_I_ST:
				cpu_st(B_reg, D_reg);
				break;
			case OP_I_LW:
				cpu_lw(B_reg, D_reg, Address);
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
	/**************************************************************************
	 * }}} End of Immediate Operations
	 *************************************************************************/


	/**************************************************************************
	 * Unconditional Operations {{{
	 *************************************************************************/
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
	/**************************************************************************
	 * }}}  End of Unconditional Operations
	 *************************************************************************/


	/**************************************************************************
	 * IO Operations{{{
	 *************************************************************************/
	inline void cpu_io_operation(instruct_t inst, instruct_t opcode, PCB* pcb) {
		instruct_t Reg1, Reg2, Address, offset;
		Reg1 =   (inst & 0x00F00000) >> (5*4);
		Reg2 =   (inst & 0x000F0000) >> (4*4);
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
	/**************************************************************************
	 * }}} End of IO operations
	 *************************************************************************/
};


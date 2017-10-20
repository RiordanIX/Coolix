#pragma once


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
#define OP_J_JMP  0x14
#define OP_I_BEQ  0x15
#define OP_I_BNE  0x16
#define OP_I_BEZ  0x17
#define OP_I_BNZ  0x18
#define OP_I_BGZ  0x19
#define OP_I_BLZ  0x1A
// }}}


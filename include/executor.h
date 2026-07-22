#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdint.h>
#include "decoder.h"
#include "cpu.h"


enum Opcode {
    OP_LUI    = 0x37,
    OP_AUIPC  = 0x17,
    OP_JAL    = 0x6F,
    OP_JALR   = 0x67,
    OP_BRANCH = 0x63,
    OP_LOAD   = 0x03,
    OP_STORE  = 0x23,
    OP_IMM    = 0x13,
    OP_REG    = 0x33,
    OP_FENCE  = 0x0F,
    OP_SYSTEM = 0x73
};


enum Funct3_Branch {
    F3_BEQ  = 0x0,
    F3_BNE  = 0x1,
    F3_BLT  = 0x4,
    F3_BGE  = 0x5,
    F3_BLTU = 0x6,
    F3_BGEU = 0x7
};

enum Funct3_Load {
    F3_LB  = 0x0,
    F3_LH  = 0x1,
    F3_LW  = 0x2,
    F3_LBU = 0x4,
    F3_LHU = 0x5
};

enum Funct3_Store {
    F3_SB = 0x0,
    F3_SH = 0x1,
    F3_SW = 0x2
};

enum Funct3_Imm {
    F3_ADDI  = 0x0,
    F3_SLLI  = 0x1,
    F3_SLTI  = 0x2,
    F3_SLTIU = 0x3,
    F3_XORI  = 0x4,
    F3_SRXI  = 0x5,   // SRLI or SRAI depending on funct7
    F3_ORI   = 0x6,
    F3_ANDI  = 0x7
};

enum Funct3_Reg {
    F3_ADD_SUB = 0x0,  // ADD or SUB depending on funct7
    F3_SLL     = 0x1,
    F3_SLT     = 0x2,
    F3_SLTU    = 0x3,
    F3_XOR     = 0x4,
    F3_SRX     = 0x5,  // SRL or SRA depending on funct7
    F3_OR      = 0x6,
    F3_AND     = 0x7
};

enum Funct7 {
    F7_NORMAL = 0x00,  // ADD, SRL, SRLI
    F7_ALT    = 0x20   // SUB, SRA, SRAI
};

enum InstructionType {
    INSTR_LUI,
    INSTR_AUIPC,
    INSTR_JAL,
    INSTR_JALR,
    INSTR_BEQ,  INSTR_BNE,  INSTR_BLT,  INSTR_BGE,  INSTR_BLTU, INSTR_BGEU,
    INSTR_LB,   INSTR_LH,   INSTR_LW,   INSTR_LBU,  INSTR_LHU,
    INSTR_SB,   INSTR_SH,   INSTR_SW,
    INSTR_ADDI, INSTR_SLTI, INSTR_SLTIU, INSTR_XORI, INSTR_ORI,  INSTR_ANDI,
    INSTR_SLLI, INSTR_SRLI, INSTR_SRAI,
    INSTR_ADD,  INSTR_SUB,  INSTR_SLL,  INSTR_SLT,  INSTR_SLTU,
    INSTR_XOR,  INSTR_SRL,  INSTR_SRA,  INSTR_OR,   INSTR_AND,
    INSTR_FENCE,
    INSTR_ECALL,
    INSTR_EBREAK,
    INSTR_UNKNOWN
};


enum InstructionType decode_instruction_type(instr_fields fields);
void execute_instruction(CPU *cpu, instr_fields fields , enum InstructionType);

#endif

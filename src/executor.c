#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/executor.h"
#include "cpu.h"
#include "decoder.h"

enum InstructionType decode_instruction_type(instr_fields f){

    switch (f.opcode) {
        case OP_LUI :
            return INSTR_LUI; //U-Type
            break;
        case  OP_AUIPC :
            return INSTR_AUIPC; //U-Type
            break;
        case OP_JAL :
            return INSTR_JAL; //J-Type
            break;
        case OP_JALR :
            return INSTR_JALR; //I-Type
            break;
        case OP_BRANCH : //B-Type so we will need to differentiate using funct 3
            switch(f.funct3) {
                case F3_BEQ:
                    return INSTR_BEQ;
                    break;
                case F3_BNE:
                    return INSTR_BNE;
                    break;
                case F3_BLT:
                    return INSTR_BLT;
                    break;
                case F3_BGE:
                    return INSTR_BGE;
                    break;
                case F3_BLTU:
                    return INSTR_BLTU;
                    break;
                case F3_BGEU:
                    return INSTR_BGEU;
                    break;
                }
        case OP_LOAD : //I-Type so we will need to differentiate using funct 3 again
            switch(f.funct3) {
                case F3_LB :
                    return INSTR_LB;
                    break;
                case F3_LH :
                    return INSTR_LH;
                    break;
                case F3_LW :
                    return INSTR_LW;
                    break;
                case F3_LBU :
                    return INSTR_LBU;
                    break;
                case F3_LHU :
                    return INSTR_LHU;
                    break;
            }
        case OP_STORE :  //S-Type so we will need to differentiate using funct 3 again
            switch(f.funct3){
                case F3_SB :
                    return INSTR_SB;
                    break;
                case F3_SH :
                    return INSTR_SH;
                    break;
                case F3_SW :
                    return INSTR_SW;
                    break;
            }
        case OP_IMM :
            switch(f.funct3){
                case F3_ADDI :
                    return INSTR_ADDI;
                    break;
                case F3_SLLI :
                    return INSTR_SLLI;
                    break;
                case F3_SLTI :
                    return INSTR_SLTI;
                    break;
                case F3_SLTIU :
                    return INSTR_SLTIU;
                    break;
                case F3_XORI :
                    return INSTR_XORI;
                    break;
                case F3_SRXI :
                    switch(f.funct7){
                        case F7_NORMAL :
                            return INSTR_SRLI;
                            break;
                        case F7_ALT :
                            return INSTR_SRAI;
                            break;
                    }
                case F3_ORI :
                    return INSTR_ORI;
                    break;
                case F3_ANDI :
                    return INSTR_ANDI;
                    break;

            }
        case OP_REG :
            switch(f.funct3){
                case F3_ADD_SUB :
                    switch (f.funct7) {
                        case F7_NORMAL :
                            return INSTR_ADD;
                            break;
                        case F7_ALT :
                            return INSTR_SUB;
                            break;
                    }
                case F3_SLL :
                    return INSTR_SLL;
                    break;
                case F3_SLT :
                    return INSTR_SLT;
                    break;
                case F3_SLTU :
                    return INSTR_SLTU;
                    break;
                case F3_XOR :
                    return INSTR_XOR;
                    break;
                case F3_SRX :
                    switch (f.funct7){
                        case F7_NORMAL:
                            return INSTR_SRL;
                            break;
                        case F7_ALT:
                            return INSTR_SRA;
                            break;
                }
                case F3_OR :
                    return INSTR_OR;
                    break;
                case F3_AND :
                    return INSTR_AND;
                    break;
            }
            case OP_FENCE :
                return INSTR_FENCE;
                break;
            case OP_SYSTEM :
                if ((f.imm & 0xFFF) == 0x000) return INSTR_ECALL;
                if ((f.imm & 0xFFF) == 0x001) return INSTR_EBREAK;
                return INSTR_UNKNOWN;

            default :
                return INSTR_UNKNOWN;
    }

}


static void exec_lui(CPU * cpu , instr_fields f){

    uint32_t value = f.imm;
    uint8_t index = f.rd;
    cpu_write_register(index, cpu, value);
    //loads up the immediate value into the destination register rd
}

static void exec_auipc(CPU * cpu , instr_fields f){

    uint32_t value = f.imm;
    uint8_t index = f.rd;
    uint32_t curr_pc = cpu->pc;
    uint32_t new_pc = curr_pc + value;
    cpu_write_register(index, cpu, new_pc);
    //adds the immediate value to the current PC address

}

static void exec_jal(CPU * cpu , instr_fields f){

    uint8_t index = f.rd;
    uint32_t imm = f.imm;
    uint32_t offset = cpu->pc + imm;
    cpu_write_register(index, cpu, offset);



    //jal stores pc+4 into rd
    // jumps to pc+imm (the offset is relative to the current PC)

}


void execute_instruction(CPU *cpu, instr_fields fields , enum InstructionType);

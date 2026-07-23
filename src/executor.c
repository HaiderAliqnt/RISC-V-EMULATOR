#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/executor.h"
#include "cpu.h"
#include "decoder.h"
#include "memory.h"

static void exec_lui(CPU * cpu , instr_fields f){

    uint32_t value = f.imm;
    uint8_t index = f.rd;
    cpu_write_register(index, cpu, value);
    cpu->pc+=4;
    //loads up the immediate value into the destination register rd
}

static void exec_auipc(CPU * cpu , instr_fields f){

    uint32_t value = f.imm;
    uint8_t index = f.rd;
    uint32_t curr_pc = cpu->pc;
    uint32_t new_pc = curr_pc + value;
    cpu_write_register(index, cpu, new_pc);
    cpu->pc+=4;
    //adds the immediate value to the current PC address

}

static void exec_jal(CPU * cpu , instr_fields f){

    uint32_t value = (cpu->pc + 4);
    cpu_write_register(f.rd, cpu, value) ; //writes the pc+4 into rd
    cpu->pc += f.imm; //updates the pc value bu adding it to the imm thus jumping to that position

    //jal stores pc+4 into rd so first we are going to write the value pc + 4 into rd
    // jumps to pc+imm (the offset is relative to the current PC)
    // we will not add pc+=4 to jump instr cus we already have an adress to jump to
}

static void exec_jalr(CPU * cpu , instr_fields f){

    uint32_t value = (cpu->pc + 4);
    cpu_write_register(f.rd , cpu, value); //writed pc+4 into rd
    uint32_t target = (f.imm + f.rs1); //calculate target
    cpu->pc = target; //now give pc that value so it can jump onto it


    //jalr again stores pc+4 into rd
    // instead of adding the imm to the pc we will add it to rs
    // we will then jump to that value
}

static void exec_beq(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = cpu_read_register(f.rs2, cpu);

    if(val1 == val2){ //if this condition is true the branch will be taken
        cpu->pc = cpu->pc + f.imm;
    }
    else{
        cpu->pc += 4;
    }
}

static void exec_bne(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = cpu_read_register(f.rs2, cpu);

    if(val1 != val2){ //if this condition is true the branch will be taken
        cpu->pc = cpu->pc + f.imm;
    }
    else{
        cpu->pc += 4;
    }
}

static void exec_blt(CPU * cpu , instr_fields f){

    int32_t val1 = cpu_read_register(f.rs1, cpu);
    int32_t val2 = cpu_read_register(f.rs2, cpu);

    if(val1 < val2){ //if this condition is true the branch will be taken
        cpu->pc = cpu->pc + f.imm;
    }
    else{
        cpu->pc += 4;
    }
}

static void exec_bge(CPU * cpu , instr_fields f){

    int32_t val1 = cpu_read_register(f.rs1, cpu);
    int32_t val2 = cpu_read_register(f.rs2, cpu);

    if(val1 >= val2){ //if this condition is true the branch will be taken
        cpu->pc = cpu->pc + f.imm;
    }
    else{
        cpu->pc += 4;
    }
}

static void exec_bltu(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = cpu_read_register(f.rs2, cpu);

    if(val1 < val2){ //if this condition is true the branch will be taken
        cpu->pc = cpu->pc + f.imm;
    }
    else{
        cpu->pc += 4;
    }
}

static void exec_bgeu(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = cpu_read_register(f.rs2, cpu);

    if(val1 >= val2){ //if this condition is true the branch will be taken
        cpu->pc = cpu->pc + f.imm;
    }
    else{
        cpu->pc += 4;
    }
}

static void exec_lb(CPU * cpu , instr_fields f){

    uint32_t target_addr = cpu_read_register(f.rs1 , cpu) + f.imm ;
    uint8_t value = memory_read8(target_addr); //we are just reading a byte that is 8 bits
    int32_t sig_ext_val = (int32_t)(int8_t)value; //we are going to sign extend it from bit 7
    cpu_write_register(f.rd, cpu , (uint32_t)sig_ext_val); //then pass the unsigned version to the write function
    cpu->pc += 4;

}

static void exec_lh(CPU * cpu , instr_fields f){

    uint32_t target_addr = cpu_read_register(f.rs1 , cpu) + f.imm ;
    uint16_t value = memory_read16(target_addr); //we are just reading 2 bytes that is 16 bits
    int32_t sig_ext_val = (int32_t)(int16_t)value; //we are going to sign extend it from bit 15
    cpu_write_register(f.rd, cpu , (uint32_t)sig_ext_val); //then pass the unsigned version to the write function
    cpu->pc += 4;

}

static void exec_lw(CPU * cpu , instr_fields f){

    uint32_t target_addr = cpu_read_register(f.rs1 , cpu) + f.imm ;
    uint32_t value = memory_read32(target_addr); //we are just reading 4 bytes that is 32 bits
    cpu_write_register(f.rd, cpu , value); //no sign extension required
    cpu->pc += 4;

}

static void exec_lbu(CPU * cpu , instr_fields f){

    uint32_t target_addr = cpu_read_register(f.rs1 , cpu) + f.imm ;
    uint32_t value = (uint32_t)memory_read8(target_addr); //we are just reading 1 byte that is 8 bits unsigned hence 0 gets extended to 32 bits
    cpu_write_register(f.rd, cpu , value); //no sign extension required
    cpu->pc += 4;

}

static void exec_lhu(CPU * cpu , instr_fields f){

    uint32_t target_addr = cpu_read_register(f.rs1 , cpu) + f.imm ;
    uint32_t value = (uint32_t)memory_read16(target_addr); //we are reading 2 bytes and then we sign extend them automatically to 0 since lhu deals with unsigned
    cpu_write_register(f.rd, cpu , value); //no sign extension required
    cpu->pc += 4;

}

static void exec_sb(CPU * cpu , instr_fields f){

    uint32_t target_addr = cpu_read_register(f.rs1 , cpu) + f.imm ; //same as before
    uint8_t value = (cpu_read_register( f.rs2 , cpu)) & 0xFF; //the value is extracted from the register and masked to only extract 8 bits (a byte)
    memory_write8(target_addr , value);
    cpu->pc += 4;

}

static void exec_sh(CPU * cpu , instr_fields f){

    uint32_t target_addr = cpu_read_register(f.rs1 , cpu) + f.imm ; //same as before
    uint16_t value = (cpu_read_register( f.rs2 , cpu)) & 0xFFFF; //the value is extracted from the register and masked to only extract 16 bits (2 bytes which is half word)
    memory_write16(target_addr , value);
    cpu->pc += 4;

}

static void exec_sw(CPU * cpu , instr_fields f){

    uint32_t target_addr = cpu_read_register(f.rs1 , cpu) + f.imm ; //same as before
    uint32_t value = (cpu_read_register( f.rs2 , cpu)); //the value is extracted from the register with no mask since all bits are taken into 4 bytes a word
    memory_write32(target_addr , value);
    cpu->pc += 4;

}

static void exec_addi(CPU * cpu , instr_fields f){

    int32_t val1 = (int32_t)cpu_read_register(f.rs1, cpu);
    int32_t ans = val1 + f.imm;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}

static void exec_slti(CPU * cpu , instr_fields f){

    int32_t val1 = (int32_t)cpu_read_register(f.rs1, cpu);

    if(val1 < f.imm){
        cpu_write_register(f.rd, cpu, 1);
    }
    else{
        cpu_write_register(f.rd, cpu, 0);
    }

    cpu->pc += 4;
}

static void exec_sltiu(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t uimm = (uint32_t)f.imm;
    if(val1 < uimm){
        cpu_write_register(f.rd, cpu, 1);
    }
    else{
        cpu_write_register(f.rd, cpu, 0);
    }

    cpu->pc += 4;
}

static void exec_xori(CPU * cpu , instr_fields f){

    uint32_t val1 = (uint32_t)cpu_read_register(f.rs1, cpu);
    uint32_t ans = val1 ^ f.imm;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}

static void exec_ori(CPU * cpu , instr_fields f){

    uint32_t val1 = (uint32_t)cpu_read_register(f.rs1, cpu);
    uint32_t ans = val1 | f.imm;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}

static void exec_andi(CPU * cpu , instr_fields f){

    uint32_t val1 = (uint32_t)cpu_read_register(f.rs1, cpu);
    uint32_t ans = val1 & f.imm;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}

static void exec_slli(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t shift_amt = f.imm & 0x1F; //we only need the lower 5 bits of the immediate not the whole hence the mask
    cpu_write_register(f.rd, cpu, val1 << shift_amt);
    cpu->pc += 4;

}

static void exec_srli(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t shift_amt = f.imm & 0x1F; //we only need the lower 5 bits of the immediate not the whole hence the mask
    cpu_write_register(f.rd, cpu, val1 >> shift_amt);
    cpu->pc += 4;

}


static void exec_srai(CPU * cpu , instr_fields f){

    int32_t val1 = (int32_t)cpu_read_register(f.rs1, cpu);
    uint32_t shift_amt = f.imm & 0x1F; //we only need the lower 5 bits of the immediate not the whole hence the mask
    cpu_write_register(f.rd, cpu, (uint32_t)(val1 >> shift_amt));//srai deals with signed bits hence we need to scape it
    cpu->pc += 4;

}

static void exec_add(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = cpu_read_register(f.rs2, cpu);
    uint32_t ans = val1 + val2;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}

static void exec_sub(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = cpu_read_register(f.rs2, cpu);
    uint32_t ans = val1 - val2;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}

static void exec_or(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = cpu_read_register(f.rs2, cpu);
    uint32_t ans = val1 | val2;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}

static void exec_and(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = cpu_read_register(f.rs2, cpu);
    uint32_t ans = val1 & val2;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}

static void exec_xor(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = cpu_read_register(f.rs2, cpu);
    uint32_t ans = val1 ^ val2;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}

static void exec_sll(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = (cpu_read_register(f.rs2, cpu)) & 0x1F; //masked for 5 bits only
    uint32_t ans = val1 << val2;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}

static void exec_srl(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = (cpu_read_register(f.rs2, cpu)) & 0x1F; //masked for 5 bits only
    uint32_t ans = val1 >> val2;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}

static void exec_sra(CPU * cpu , instr_fields f){

    int32_t val1 = (int32_t)cpu_read_register(f.rs1, cpu);
    uint32_t val2 = (uint32_t)(cpu_read_register(f.rs2, cpu)) & 0x1F; //masked for 5 bits only
    uint32_t ans = val1 >> val2;
    cpu_write_register(f.rd, cpu, ans);
    cpu->pc += 4;

}


static void exec_sltu(CPU * cpu , instr_fields f){

    uint32_t val1 = cpu_read_register(f.rs1, cpu);
    uint32_t val2 = cpu_read_register(f.rs2, cpu);
    if(val1 < val2){
        cpu_write_register(f.rd, cpu,1);
    }
    else{
        cpu_write_register(f.rd, cpu,0);
    }
    cpu->pc += 4;

}

static void exec_slt(CPU * cpu , instr_fields f){

    int32_t val1 = (int32_t)cpu_read_register(f.rs1, cpu);
    int32_t val2 = (int32_t)cpu_read_register(f.rs2, cpu);
    if(val1 < val2){
        cpu_write_register(f.rd, cpu,1);
    }
    else{
        cpu_write_register(f.rd, cpu,0);
    }
    cpu->pc += 4;
}

static void exec_fence(CPU *cpu) {
    cpu->pc += 4; // no-op for single core
}

static void exec_ecall(CPU *cpu) {
    printf("ECALL at PC: 0x%08X\n", cpu->pc);
    exit(0);  // clean halt for now
}

static void exec_ebreak(CPU *cpu) {
    printf("EBREAK at PC: 0x%08X\n", cpu->pc);
    exit(0);  // clean halt for now
}

void execute_instruction(CPU *cpu, instr_fields f) {
    switch (f.opcode) {
        case OP_LUI:
            exec_lui(cpu, f);
            break;
        case OP_AUIPC:
            exec_auipc(cpu, f);
            break;
        case OP_JAL:
            exec_jal(cpu, f);
            break;
        case OP_JALR:
            exec_jalr(cpu, f);
            break;
        case OP_BRANCH:
            switch (f.funct3) {
                case F3_BEQ:  exec_beq(cpu, f);  break;
                case F3_BNE:  exec_bne(cpu, f);  break;
                case F3_BLT:  exec_blt(cpu, f);  break;
                case F3_BGE:  exec_bge(cpu, f);  break;
                case F3_BLTU: exec_bltu(cpu, f); break;
                case F3_BGEU: exec_bgeu(cpu, f); break;
                default:
                    printf("Unknown branch funct3: 0x%02X at PC: 0x%08X\n", f.funct3, cpu->pc);
                    exit(1);
            }
            break;
        case OP_LOAD:
            switch (f.funct3) {
                case F3_LB:  exec_lb(cpu, f);  break;
                case F3_LH:  exec_lh(cpu, f);  break;
                case F3_LW:  exec_lw(cpu, f);  break;
                case F3_LBU: exec_lbu(cpu, f); break;
                case F3_LHU: exec_lhu(cpu, f); break;
                default:
                    printf("Unknown load funct3: 0x%02X at PC: 0x%08X\n", f.funct3, cpu->pc);
                    exit(1);
            }
            break;
        case OP_STORE:
            switch (f.funct3) {
                case F3_SB: exec_sb(cpu, f); break;
                case F3_SH: exec_sh(cpu, f); break;
                case F3_SW: exec_sw(cpu, f); break;
                default:
                    printf("Unknown store funct3: 0x%02X at PC: 0x%08X\n", f.funct3, cpu->pc);
                    exit(1);
            }
            break;
        case OP_IMM:
            switch (f.funct3) {
                case F3_ADDI:  exec_addi(cpu, f);  break;
                case F3_SLLI:  exec_slli(cpu, f);  break;
                case F3_SLTI:  exec_slti(cpu, f);  break;
                case F3_SLTIU: exec_sltiu(cpu, f); break;
                case F3_XORI:  exec_xori(cpu, f);  break;
                case F3_SRXI:
                    switch (f.funct7) {
                        case F7_NORMAL: exec_srli(cpu, f); break;
                        case F7_ALT:    exec_srai(cpu, f); break;
                        default:
                            printf("Unknown funct7 for SRXI: 0x%02X at PC: 0x%08X\n", f.funct7, cpu->pc);
                            exit(1);
                    }
                    break;
                case F3_ORI:  exec_ori(cpu, f);  break;
                case F3_ANDI: exec_andi(cpu, f); break;
                default:
                    printf("Unknown IMM funct3: 0x%02X at PC: 0x%08X\n", f.funct3, cpu->pc);
                    exit(1);
            }
            break;
        case OP_REG:
            switch (f.funct3) {
                case F3_ADD_SUB:
                    switch (f.funct7) {
                        case F7_NORMAL: exec_add(cpu, f); break;
                        case F7_ALT:    exec_sub(cpu, f); break;
                        default:
                            printf("Unknown funct7 for ADD/SUB: 0x%02X at PC: 0x%08X\n", f.funct7, cpu->pc);
                            exit(1);
                    }
                    break;
                case F3_SLL:  exec_sll(cpu, f);  break;
                case F3_SLT:  exec_slt(cpu, f);  break;
                case F3_SLTU: exec_sltu(cpu, f); break;
                case F3_XOR:  exec_xor(cpu, f);  break;
                case F3_SRX:
                    switch (f.funct7) {
                        case F7_NORMAL: exec_srl(cpu, f); break;
                        case F7_ALT:    exec_sra(cpu, f); break;
                        default:
                            printf("Unknown funct7 for SRL/SRA: 0x%02X at PC: 0x%08X\n", f.funct7, cpu->pc);
                            exit(1);
                    }
                    break;
                case F3_OR:  exec_or(cpu, f);  break;
                case F3_AND: exec_and(cpu, f); break;
                default:
                    printf("Unknown REG funct3: 0x%02X at PC: 0x%08X\n", f.funct3, cpu->pc);
                    exit(1);
            }
            break;
        case OP_FENCE:
            exec_fence(cpu);
            break;
        case OP_SYSTEM:
            if ((f.imm & 0xFFF) == 0x000) exec_ecall(cpu);
            else if ((f.imm & 0xFFF) == 0x001) exec_ebreak(cpu);
            else {
                printf("Unknown SYSTEM instruction at PC: 0x%08X\n", cpu->pc);
                exit(1);
            }
            break;
        default:
            printf("Unknown opcode: 0x%02X at PC: 0x%08X\n", f.opcode, cpu->pc);
            exit(1);
    }
}

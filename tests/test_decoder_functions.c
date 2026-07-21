#include <stdint.h>
#include <stdio.h>
#include "../include/decoder.h"


int main(){

    printf("Decoder testing in progress...\n");

    instr_fields f = {0};

    //hardcoded instructions
    uint32_t hardcoded_test_instructions [] = {
        0x003100B3,  // R - ADD
        0x00510093,  // I - ADDI
        0x00212423,  // S - SW
        0x00208463,  // B - BEQ
        0x123450B7,  // U - LUI
        0x008000EF   // J - JAL
    };

    for (int i = 0; i < 6; i++) {
        f = decoder_function(hardcoded_test_instructions[i]);
        printf("opcode=0x%02x rd=%d funct3=%d rs1=%d rs2=%d funct7=%d imm=%d\n",
               f.opcode, f.rd, f.funct3, f.rs1, f.rs2, f.funct7, f.imm);
    }
}

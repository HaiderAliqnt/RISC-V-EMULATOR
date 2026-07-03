#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>

typedef struct {
    uint8_t opcode; //[6:0]
    uint8_t rd; //[11:7]
    uint8_t funct3; //[14:12]
    uint8_t rs1; //[19:15]
    uint8_t rs2; //[24:20]
    uint8_t funct7; //[31:25]
    int32_t imm;
} instr_fields;

struct instr_fields decoder_function(uint32_t instruction);




#endif

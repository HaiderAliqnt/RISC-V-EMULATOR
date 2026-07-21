#include "../include/decoder.h"
#include <stdint.h>
#include <stdio.h>

static int32_t sign_extend(uint32_t value, uint8_t bit){
    //we are gonna check over here if the MSB which is the 12th bit in the imm == 1
    if(value & (1 << bit)){ //if this returns true that means MSB is 1
        value |=0xFFFFF000;  //using OR we are going to fill the remaining bits with 1
    }
    return value;
}

static int32_t decode_imm_i(uint32_t instruction){

    int32_t imm = (instruction >> 20) & ((1 << 12) - 1);
    int32_t sig_ext_imm = sign_extend(imm,11);
    return sig_ext_imm;

}

static int32_t decode_imm_s(uint32_t instruction){
    //the instruction gives us immediate in 2 parts , one part is imm[4:0] from bits 11:7 and the other is  imm[11:5] from bits 31:25

    int32_t imm = (instruction >> 7) & ((1 << 5) -1 );
    imm  |= ((instruction >> 25) & ((1 << 7) -1)) << 5; //shifts the 5 lower bits to the left to make space for the 7 bits and then ORs
    int32_t sig_ext_imm = sign_extend(imm, 11);
    return  sig_ext_imm;

}

static int32_t decode_imm_u(uint32_t instruction){
    //imm is bits 31:12 shifted left by 12, lower 12 bits are zero ...apply the mask directly since the bits have already been shifted
    //no sign extension required since the lower bits are zeroed out
    int32_t imm = ((instruction) & ((1 << 20) - 1));
    return imm;
}

static int32_t decode_imm_b(uint32_t instruction){
    // bit 0 is always 0
    // imm[4:1]  from bits 11:8
    // imm[10:5] from bits 30:25
    // imm[11]   from bit  7
    // imm[12]   from bit  31
    // sign extend from bit 12
    int32_t imm = ((instruction >> 8) & ((1 << 4) -1 )) << 1;
    imm |= ((instruction >> 25) & ((1 << 6) -1)) << 5;
    imm |= ((instruction >> 7) & ((1 << 1) -1)) << 11;
    imm |= ((instruction >> 31) & ((1 << 1) - 1)) << 12;

    int32_t sig_ext_imm = sign_extend(imm, 12);
    return sig_ext_imm;

}

static int32_t decode_imm_j(uint32_t instruction){
    // bit 0 is always 0
    // imm[10:1]  from bits 30:21
    // imm[11]    from bit  20
    // imm[19:12] from bits 19:12
    // imm[20]    from bit  31
    // sign extend from bit 20
    int32_t imm = ((instruction >> 21) & ((1 << 10) - 1)) << 1;
    imm |= ((instruction >> 20) & ((1 << 1) -1)) << 11;
    imm |= ((instruction >> 12) & ((1 << 8) -1)) << 12;
    imm |= ((instruction >> 30) & ((1 << 1) -1)) << 20;

    int32_t sig_ext_imm = sign_extend(imm, 20);
    return sig_ext_imm;
}





instr_fields decoder_function(uint32_t instruction){

    //first we will extract all the gen fields that are consistent for evry instr

     instr_fields fields = {0};
     fields.opcode = (instruction)  & 0x7F ;  //0111 1111
     fields.rd = (instruction >> 7) & 0x1F ;  //0001 1111
     fields.funct3 = (instruction >> 12) & 0x07 ; //0000 0111
     fields.rs1 = (instruction >> 15) & 0x1F ; //0001 1111
     fields.rs2 = (instruction >> 20) & 0x1F ; //0001 1111
     fields.funct7 = (instruction >> 25) & 0x7F ; //0111 1111

     //using a switch case statement to assign immediates if needed
     //for same instruction types but diff opcode we use fallbacks to avoid using same function twice and creating duplicates
     switch (fields.opcode) {
         case 0x37 : //U-TYPE
         case 0x17 : //U-TYPE
             fields.imm = decode_imm_u(instruction);
             break;
         ;
         case 0x6F : //J-TYPE
            fields.imm = decode_imm_j(instruction);
            break;
        ;
         case 0x67: //I-TYPE
         case 0x13 : //I-TYPE
         case 0x03: //I-TYPE
             fields.imm = decode_imm_i(instruction);
             break;

         case 0x63: //B-TYPE
            fields.imm = decode_imm_b(instruction);
            break;

         case 0x23: //S-TYPE
            fields.imm = decode_imm_s(instruction);
            break;


         case 0x33 : //R-TYPE
            //no immediates assigned
            break;

        default:
             printf("Unknown opcode: 0x%02x\n", fields.opcode);
             break;

     }

     return fields;
}

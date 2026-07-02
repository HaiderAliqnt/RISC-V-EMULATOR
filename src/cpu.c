#include "../include/cpu.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


void cpu_init(CPU * cpu){

    memset(cpu->regs , 0 , sizeof(cpu->regs)); //memset basically just assigns the given value to n btis of memory
    cpu->pc = 0X000000000;
}

uint32_t cpu_read_register(uint8_t index, CPU * cpu){

    if (index == 0){
        return 0;
    }
    return cpu->regs[index];
}

void cpu_write_register(uint8_t index, CPU * cpu, uint32_t value){

    if(index == 0){
        printf("Can Not write to register 0");
    }
    cpu->regs[index] = value;
}

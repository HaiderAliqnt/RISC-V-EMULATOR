#include "../include/memory.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static uint8_t *mem = NULL;

static int handle_mmio_write(uint32_t address, uint32_t value) {
    if (address == UART_BASE) {
        putchar((char)(value & 0xFF));  // UART output... prints the character
        fflush(stdout);
        return 1;  // handled
    }
    return 0;  // not an MMIO address
}


void memory_init(void){

    mem = (uint8_t *)malloc(MEMORY_SIZE);
    if(mem == NULL){
        fprintf(stderr , "Failed to allocate memory");
    }
    memset(mem , 0 , MEMORY_SIZE); //zero everything out
}

void memory_free(void){

    if(mem!=NULL){
        free(mem);
        mem = NULL;
    }
    printf("Nothing in memory to free");
}

static void check_bounds(uint32_t address , uint32_t size){

    if(address + size > MEMORY_SIZE){
        fprintf(stderr, "Memory access out of bounds: 0x%08x\n", address);
        exit(1);
    }
}

uint8_t memory_read8(uint32_t address){

    check_bounds(address , 1);
    return mem[address];

}

uint16_t memory_read16(uint32_t address){

    check_bounds(address, 2);
    return (uint16_t)(mem[address])|
    (uint16_t)(mem[address+1] << 8);

}

uint32_t memory_read32(uint32_t address){

    check_bounds(address, 4);
    return  (uint32_t)(mem[address])|
            (uint32_t)(mem[address+1]<<8)|
            (uint32_t)(mem[address+2]<<16)|
            (uint32_t)(mem[address+3]<<24);

}

void  memory_write8(uint32_t address, uint8_t value){

    if (handle_mmio_write(address, value)) return;
    check_bounds(address, 1);
    mem[address] = value;

}

void memory_write16(uint32_t address, uint16_t value){

    if (handle_mmio_write(address, value)) return;
    check_bounds(address , 2);
    mem[address] = (uint8_t)(value & 0XFF);
    mem[address+1] = (uint8_t)((value>>8) & 0XFF);

}

void memory_write32(uint32_t address, uint32_t value){

    if (handle_mmio_write(address, value)) return;
    check_bounds(address , 4);
    mem[address] = (uint8_t)(value & 0XFF);
    mem[address+1] = (uint8_t)((value>>8) & 0XFF);
    mem[address+2] = (uint8_t)((value>>16) & 0XFF);
    mem[address+3] = (uint8_t)((value>>24) & 0XFF);

}

void memory_load(uint32_t address, uint8_t *data, uint32_t size){

    check_bounds(address, size);
    memcpy(mem+address , data, size);

}

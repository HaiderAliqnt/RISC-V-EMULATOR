#include "../include/memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static uint8_t *mem = NULL;

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



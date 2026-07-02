#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct {
    uint32_t regs [32];
    uint32_t pc;
} CPU;

void cpu_init (CPU * cpu);
uint32_t cpu_read_register(uint8_t index , CPU * cpu);
void cpu_write_register(uint8_t index , CPU * cpu , uint32_t value);



#endif

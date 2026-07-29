// # x1 = counter (1 to 5)
// # x2 = accumulator (sum)
// # x3 = limit (5)

// addi x1, x0, 1      # x1 = 1
// addi x2, x0, 0      # x2 = 0
// addi x3, x0, 5      # x3 = 5

// loop:
// add  x2, x2, x1     # x2 = x2 + x1
// addi x1, x1, 1      # x1 = x1 + 1
// ble  x1, x3, loop   # if x1 <= x3 go back to loop

// # when done x2 should contain 15 (1+2+3+4+5)
// ebreak               # halt


#include <stdint.h>
#include <stdio.h>
#include "../include/memory.h"
#include "../include/cpu.h"
#include "../include/decoder.h"
#include "../include/executor.h"

int main(void) {
    uint8_t program[] = {
        0x93, 0x00, 0x10, 0x00,  // addi x1, x0, 1
        0x13, 0x01, 0x00, 0x00,  // addi x2, x0, 0
        0x93, 0x01, 0x60, 0x00,  // addi x3, x0, 6
        0x33, 0x01, 0x11, 0x00,  // add x2, x2, x1
        0x93, 0x80, 0x10, 0x00,  // addi x1, x1, 1
        0xE3, 0xCC, 0x30, 0xFE,  // blt x1, x3, -8
        0x73, 0x00, 0x10, 0x00   // ebreak
    };
    // uint8_t program[] = {
    //     0x93, 0x00, 0x10, 0x00,  // addi x1, x0, 1
    //     0x13, 0x01, 0x20, 0x00,  // addi x2, x0, 2
    //     0x93, 0x01, 0x30, 0x00,  // addi x3, x0, 3
    //     0xB3, 0x80, 0x20, 0x00,  // add  x1, x1, x2    → x1 = 3
    //     0x33, 0x01, 0x31, 0x00,  // add  x2, x2, x3    → x2 = 5
    //     0xB3, 0x81, 0x20, 0x00,  // add  x3, x1, x2    → x3 = 8  (fibonacci-ish)
    //     0x13, 0x09, 0x50, 0x00,  // addi x18, x0, 5    → x18 = 5
    //     0x93, 0x09, 0x30, 0x00,  // addi x19, x0, 3    → x19 = 3
    //     0x33, 0x0A, 0x39, 0x01,  // add  x20, x18, x19 → x20 = 8
    //     0x73, 0x00, 0x10, 0x00   // ebreak
    // };

    CPU cpu;
    memory_init();
    cpu_init(&cpu);

    memory_load(0x0000, program, sizeof(program));
    cpu.pc = 0x0000;

    while (1) {
        printf("\nPC: 0x%08X  raw: 0x%08X\n", cpu.pc, memory_read32(cpu.pc));
        uint32_t raw = memory_read32(cpu.pc);
        instr_fields fields = decoder_function(raw);
        execute_instruction(&cpu, fields);
        printf("x1=%d x2=%d x3=%d\n",
               cpu_read_register(1, &cpu),
               cpu_read_register(2, &cpu),
               cpu_read_register(3, &cpu));
         // printf("x1=%d x2=%d x3=%d x18=%d x19=%d x20=%d\n",
         //       cpu_read_register(1,  &cpu),
         //       cpu_read_register(2,  &cpu),
         //       cpu_read_register(3,  &cpu),
         //       cpu_read_register(18, &cpu),
         //       cpu_read_register(19, &cpu),
         //       cpu_read_register(20, &cpu));
    }

    memory_free();
    return 0;
}

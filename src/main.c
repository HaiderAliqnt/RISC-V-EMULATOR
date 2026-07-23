#include <stdio.h>
#include <stdint.h>
#include "cpu.h"
#include "memory.h"
#include "decoder.h"
#include "executor.h"

int main(void) {
    printf("RISC-V Emulator starting...\n");

    CPU cpu;
    memory_init();
    cpu_init(&cpu);

    while (1) {
        uint32_t raw = memory_read32(cpu.pc);
        instr_fields fields = decoder_function(raw);
        execute_instruction(&cpu, fields);

    }

    memory_free();
    return 0;
}

#include <stdio.h>
#include <stdint.h>
#include "cpu.h"
#include "elf-loader.h"
#include "memory.h"
#include "decoder.h"
#include "executor.h"


int main(int argc , char* argv[]) {

    if(argc > 2){
        printf("Usage: emulator <elf file>\n");
        return 1;
    }

    printf("RISC-V Emulator starting...\n");

    CPU cpu;
    memory_init();
    cpu_init(&cpu);
    elf_load(argv[1], &cpu);

    printf("Loaded ELF — entry point: 0x%08X\n", cpu.pc);

    while (1) {
        uint32_t raw = memory_read32(cpu.pc);
        instr_fields fields = decoder_function(raw);
        execute_instruction(&cpu, fields);
    }

    memory_free();
    return 0;
}

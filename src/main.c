#include <stdio.h>
#include <stdint.h>
#include "cpu.h"
#include "elf-loader.h"
#include "memory.h"
#include "decoder.h"
#include "executor.h"
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: emulator <elf file>\n");
        return 1;
    }

    printf("RISC-V Emulator starting...\n");

    CPU cpu;
    memory_init();
    cpu_init(&cpu);
    elf_load(argv[1], &cpu);

    printf("Loaded ELF — entry point: 0x%08X\n", cpu.pc);

    int running = 1;

    while (running) {


        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        memory_set_current_pc(cpu.pc);
        if (cpu.pc == 0x1e414) {
            printf("[MALLOC RETURN] a0=0x%08x\n", cpu_read_register(10, &cpu));
        }
        uint32_t raw = memory_read32(cpu.pc);
        instr_fields fields = decoder_function(raw);
        // printf("PC=0x%08x\n", cpu.pc); //debug line REMOVE when done
        execute_instruction(&cpu, fields);
    }

    memory_free();
    SDL_Quit();

    return 0;
}

#include <stdint.h>
#include <stdio.h>
#include "../include/cpu.h"
#include "../include/memory.h"
#include "../include/decoder.h"

int main(void) {
    printf("RISC-V Emulator starting...\n");

    CPU * cpu ;
    memory_init();
    cpu_init(cpu);

    while(1){ //need to make an exit loop statement thru ECALL

        uint32_t curr_instr = memory_read32(cpu->pc);

        instr_fields fields = decoder_function(curr_instr);

        //now pas the decoded fields to executor
        //executor function call

        cpu->pc += 4;

    }

    memory_free();
    return 0;
}

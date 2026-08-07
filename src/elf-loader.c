#include "../include/elf-loader.h"
#include "../include/memory.h"
#include "../include/cpu.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

static bool elf_validate(struct Elf_Header *header) {
    if (header->mag0 != 0x7F || header->mag1 != 'E' ||
        header->mag2 != 'L'  || header->mag3 != 'F') {
        return false;
    }
    if (header->cls     != 0x01) return false;  // must be 32-bit
    if (header->data    != 0x01) return false;  // must be little endian
    if (header->machine != 0xF3) return false;  // must be RISC-V
    if (header->type    != 0x02) return false;  // must be executable
    return true;
}

static struct Execution get_exec_seg(FILE *fp) {
    struct Elf_Header header;
    struct Prog_Header prog;
    struct Execution exec_seg = {0};

    // step 1 — read and validate ELF header
    fread(&header, sizeof(header), 1, fp);
    if (!elf_validate(&header)) {
        printf("\nINVALID executable format\n");
        exit(1);
    }

    // step 2 — iterate program headers looking for PT_LOAD
    for (uint32_t i = 0; i < header.phnum; i++) {
        fseek(fp, header.phoff + (uint32_t)(sizeof(prog) * i), SEEK_SET);
        fread(&prog, sizeof(prog), 1, fp);

        if (prog.type == 0x1) {  // PT_LOAD
            exec_seg.entry        = header.entry;
            exec_seg.offset       = prog.offset;
            exec_seg.nbytes       = prog.filesz;
            exec_seg.num_mem_bytes = prog.memsz;
            exec_seg.virtual_addr = prog.virtual_addr;
            return exec_seg;
        }
    }

    printf("\nNo loadable segment found\n");
    exit(1);
}

void elf_load(const char *path, CPU *cpu) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        printf("\nFile not found: %s\n", path);
        exit(1);
    }

    struct Execution exec_seg = get_exec_seg(fp);

    fseek(fp, exec_seg.offset, SEEK_SET);
    uint8_t *data = malloc(exec_seg.nbytes);
    if (data == NULL) {
        printf("\nFailed to allocate buffer\n");
        exit(1);
    }

    fread(data, 1, exec_seg.nbytes, fp);
    memory_load(exec_seg.virtual_addr, data, exec_seg.nbytes);

    if (exec_seg.num_mem_bytes > exec_seg.nbytes) {
        memory_zero(exec_seg.virtual_addr + exec_seg.nbytes,
                    exec_seg.num_mem_bytes - exec_seg.nbytes);
    }

    cpu->pc = exec_seg.entry;
    free(data);
    fclose(fp);
}

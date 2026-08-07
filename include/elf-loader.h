#ifndef ELF_LOADER_H
#define ELF_LOADER_H

#include <stdbool.h>
#include <stdint.h>
#include "cpu.h"

struct Elf_Header {
    uint8_t  mag0, mag1, mag2, mag3, cls, data, version, osabi, abiversion;
    uint8_t  padding[7];
    uint16_t type, machine;
    uint32_t elfversion, entry, phoff, shoff, flags;
    uint16_t ehsize, phentsize, phnum, shentsize, shnum, shstrndx;
} __attribute__((packed));

struct Prog_Header {
    uint32_t type, offset, virtual_addr, paddr, filesz, memsz, flags, align;
} __attribute__((packed));

struct Execution {
    uint32_t entry, num_mem_bytes, offset, nbytes, virtual_addr;
};

void elf_load(const char *path, CPU *cpu);

#endif // ELF_LOADER_H


// mag0        → always 0x7F
// mag1        → always 0x45 ('E')
// mag2        → always 0x4C ('L')
// mag3        → always 0x46 ('F')
// cls         → class: 1 = 32-bit, 2 = 64-bit
// data        → encoding: 1 = little endian, 2 = big endian
// version     → ELF version, always 1
// osabi       → target OS/ABI, 0 = System V (bare metal)
// abiversion  → ABI version, usually 0
// padding[7]  → unused bytes to pad identification to 16 bytes total

// type        → file type: 1 = relocatable, 2 = executable, 3 = shared
// machine     → target architecture: 0xF3 = RISC-V

// elfversion  → ELF version again, always 1
// entry       → entry point address — this is what you set cpu.pc to
// phoff       → offset in file where program headers start
// flags       → architecture specific flags

// ehsize      → size of this ELF header in bytes (52 for 32-bit)
// phentsize   → size of one program header entry in bytes (32 for 32-bit)
// phnum       → number of program headers — iterate this many

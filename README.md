# RISC-V Emulator — Running DOOM

A software emulator for the RISC-V architecture written in C/C++.
The goal is to correctly execute RISC-V binaries and eventually run DOOM on top of it.

This is a learning project. Every component is built from scratch to develop a deep
understanding of how a CPU executes instructions, how memory works, and how real
software runs on real hardware.

---

## What This Project Teaches

- How a CPU fetches, decodes, and executes instructions
- RISC-V instruction formats and the base integer instruction set (RV32I)
- Bitwise operations and bit field extraction in C
- Memory layout and memory mapped I/O
- The ELF binary format and how executables are loaded
- Build systems using Make and CMake
- Low level debugging techniques

---

## Prerequisites

- Basic programming knowledge
- Data structures and algorithms
- Computer organization and assembly language basics (COAL)
- Comfort with C — pointers, memory, structs

---

## Resources

- [Reference Implementation (rvcore)](https://github.com/lalitshankarch/rvcore)
- [Programming: Principles and Practice Using C++](https://www.amazon.com/Programming-Principles-Practice-Using-C-dp-0138308683/dp/0138308683/ref=dp_ob_title_bk)
- [Modern C](https://gustedt.gitlabpages.inria.fr/modern-c/)
- [RISC-V Assembly — KU LEUVEN](https://cass-kul.github.io/)
- [Modern C++ Programming](https://github.com/federico-busato/Modern-CPP-Programming)

---

## Project Structure

```
riscv-emulator/
├── src/
│   ├── main.c           -- entry point, emulator loop
│   ├── cpu.c            -- fetch, decode, execute logic
│   ├── memory.c         -- memory read/write and memory map
│   ├── decoder.c        -- instruction format decoding
│   ├── executor.c       -- instruction execution handlers
│   └── elf_loader.c     -- ELF binary parser and loader
├── include/
│   ├── cpu.h
│   ├── memory.h
│   ├── decoder.h
│   ├── executor.h
│   └── elf_loader.h
├── tests/
│   └── test_instructions.c   -- unit tests per instruction
├── roms/
│   └── doom.wad              -- DOOM game data
├── CMakeLists.txt
└── README.md
```

---

## Build Instructions

```bash
git clone https://github.com/HaiderAliqnt/RISC-V-EMULATOR
cd riscv-emulator
mkdir build && cd build
cmake ..
make
./emulator ../roms/test.elf
```

---

## Core Concepts 

### The Fetch-Decode-Execute Cycle
Every CPU runs this loop forever. Your emulator is a software implementation of it:
1. Fetch the 32-bit instruction at the address stored in the program counter (PC)
2. Decode it — identify the instruction type and extract its fields
3. Execute it — update registers or memory accordingly
4. Advance the PC to the next instruction

### RISC-V Instruction Formats
RISC-V has six instruction formats. Every instruction is 32 bits wide. The bit layout
determines the type and encodes the source registers, destination register, and immediate
values. Decoding means extracting these fields using bitwise shifts and masks.

```
R-type:  [funct7][rs2][rs1][funct3][rd][opcode]
I-type:  [imm[11:0]][rs1][funct3][rd][opcode]
S-type:  [imm[11:5]][rs2][rs1][funct3][imm[4:0]][opcode]
B-type:  [imm[12|10:5]][rs2][rs1][funct3][imm[4:1|11]][opcode]
U-type:  [imm[31:12]][rd][opcode]
J-type:  [imm[20|10:1|11|19:12]][rd][opcode]
```

### Registers
RISC-V has 32 general purpose registers (x0 to x31). x0 is hardwired to zero — any
write to it is discarded, any read from it returns zero. The others follow a calling
convention that determines how functions pass arguments and return values.

### Memory Mapped I/O
Peripherals like UART output and display are not separate from memory — they live at
specific addresses in the memory map. Writing to address 0x10000000 might output a
character to a serial port rather than store a byte. DOOM requires this for its display.

### The ELF Format
Executables on Linux and embedded systems use the ELF format. Your emulator must parse
the ELF header to find the entry point, read the program headers to know what to load
into memory and where, then jump to the entry point to begin execution.

---

## Milestones

| Milestone | Description |
|---|---|
| Memory working | Read and write at arbitrary addresses |
| First instruction executes | ADD or ADDI completes correctly |
| All RV32I implemented | Full base instruction set passing tests |
| First ELF loaded | Simple compiled binary runs to completion |
| Hello world over UART | Output visible in terminal |
| DOOM boots | Title screen renders |
| DOOM playable | Input handling works |

---


## Author

Muhammad Haider Ali
[GitHub](https://github.com/HaiderAliqnt)
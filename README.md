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
git clone https://github.com/yourusername/riscv-emulator.git
cd riscv-emulator
mkdir build && cd build
cmake ..
make
./emulator ../roms/test.elf
```

---

## Core Concepts to Understand Before Starting

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

## Build Order — Step by Step

### Step 1 — Study the Architecture
Before writing any code, read through the RISC-V unprivileged specification and the
KU LEUVEN assembly guide. Understand all six instruction formats and the RV32I base
instruction set. Write nothing yet.

### Step 2 — Set Up the Project
Initialize the repository. Write the CMakeLists.txt. Create the directory structure.
Confirm the build system compiles an empty main.c cleanly.

### Step 3 — Implement Memory
Write memory.c. Represent memory as a flat byte array. Implement read and write
functions for 8, 16, and 32 bit accesses. Test it in isolation with hardcoded values
before connecting anything else.

### Step 4 — Implement the CPU State
Write cpu.c. Define the CPU struct — 32 registers and the program counter. Write
functions to read and write registers, enforcing the x0 zero rule. Nothing executes yet.

### Step 5 — Implement the Decoder
Write decoder.c. Take a raw 32-bit instruction integer and extract every field using
bitwise operations — opcode, rd, rs1, rs2, funct3, funct7, and each immediate format.
This is the most detail-oriented part of the project. Test each extraction function
against known instruction encodings before moving on.

### Step 6 — Implement the Fetch-Decode-Execute Loop
Wire up the loop in main.c. Fetch the instruction at the PC address from memory,
pass it to the decoder, pass the decoded fields to the executor (stub for now),
then increment the PC. Confirm the loop runs without crashing on a hardcoded instruction.

### Step 7 — Implement RV32I Instructions
Write executor.c. Implement each of the 47 base integer instructions one at a time.
Group them by type — arithmetic, logical, shift, branch, load, store, jump.
After each group, write a test in tests/ that runs a small hand-assembled binary
and checks register and memory state.

Suggested order:
- LUI, AUIPC
- JAL, JALR
- Branch instructions (BEQ, BNE, BLT, BGE, BLTU, BGEU)
- Load instructions (LB, LH, LW, LBU, LHU)
- Store instructions (SB, SH, SW)
- Arithmetic immediate (ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI)
- Arithmetic register (ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND)

### Step 8 — Run a Hand-Written Binary
Assemble a small RISC-V program by hand — a loop that adds numbers, or a fibonacci
sequence. Load it into the memory array manually. Step through it instruction by
instruction and verify the register values match your expectations. This is your first
real test that the emulator is correct.

### Step 9 — Write the ELF Loader
Write elf_loader.c. Parse the ELF header to extract the entry point address. Iterate
the program headers and copy each loadable segment into the correct memory address.
Set the PC to the entry point. Test by loading a simple ELF binary compiled with a
RISC-V cross compiler (riscv64-unknown-elf-gcc) and confirming it executes correctly.

### Step 10 — Add Memory Mapped I/O
Extend memory.c to intercept reads and writes to specific address ranges. Implement
a UART output handler at the standard address so the emulator can print characters.
Confirm by running a RISC-V hello world binary and seeing the output in the terminal.

### Step 11 — Run DOOM
Obtain a RISC-V port of DOOM compiled as an ELF binary. Add the display and input
memory mapped I/O handlers DOOM requires. Load the binary and the DOOM WAD file.
Debug until it boots. This step will surface every gap in your emulator's correctness.

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

## Notes

Do not look at the reference implementation (rvcore) until you are stuck. The value
of this project is in working through the problems yourself. Use it to verify your
approach or debug a specific issue, not as a template to follow.

Every time something breaks, use it as an opportunity to understand why before fixing it.
The bugs in this project are the learning.

---

## Author

Muhammad Haider Ali
[GitHub](https://github.com/HaiderAliqnt)
# DAILY LOGS FOR TRACKING

## WEEK 1
GOALS : STUDY THE ARCHITECTURE AND SET UP THE PROJECT (WEEK 1 AND 2 on my NOTION)

### DAY 1 :
made the file structure and intitialized a git reppo
tested the C environment
made notes from KU LEUVEN assembly guide SECTIONS 1 TILL 3

### DAY 2 :
started the RISC-V unprivileged spec, focused on instruction encoding
broke down R-type and I-type formats, made notes on opcode/rd/funct3/rs1/rs2 field layout
still not writing any code, just reading

### DAY 3 :
continued spec, covered S-type, B-type, U-type, J-type formats
made a comparison table for all six formats side by side so the field shifts make sense visually
starting to see why immediates are scrambled the way they are (sign bit placement)

### DAY 4 :
went through the full RV32I base instruction set, all 47 instructions
grouped them by category - arithmetic, logic, memory access, control flow, misc
wrote opcode/funct3/funct7 next to each one in my notes for quick reference later

### DAY 5 :
revisited KU LEUVEN guide, this time for the memory model and stack behavior
notes on stack pointer conventions, growth direction (downward), alignment rules
also refreshed modern C - stdint.h types (uint8_t, uint32_t etc), two's complement, how the number system actually maps to bit patterns
deliberately still not touching code, step 1 says read and understand only

## WEEK 2
GOALS : Implement memory and cpu module

### DAY 6 :
started memory.c
decided on flat byte array representation - uint8_t mem[MEM_SIZE]
set up the basic struct and include guards, nothing functional yet

### DAY 7 :
implemented 8-bit read and write
straightforward byte-level access, no assembly logic needed here, just indexing into the array

### DAY 8 :
implemented 16-bit read and write
had to actually think through little-endian byte assembly - low byte goes in first, high byte gets shifted left by 8 and OR'd in
write function does the reverse, mask off low byte then shift right for the high byte

### DAY 9 :
implemented 32-bit read and write
extended the same little-endian logic across 4 bytes instead of 2
this is where the bitwise reconstruction actually clicked - reading byte by byte and shifting by 0/8/16/24 then OR-ing together

### DAY 10 :
cleaned up memory.c, went back through read/write functions for consistency
still haven't done isolated testing with hardcoded values, that's next before touching cpu.c further

### DAY 11 :
started cpu.c
defined the CPU struct - 32 general purpose registers (x0 to x31) plus the program counter
kept it minimal for now, no flags or extra state beyond what step 4 asks for

### DAY 12 :
wrote the register read function
simple index into the register array, nothing tricky yet

### DAY 13 :
wrote the register write function
had to enforce the x0 hardwired-zero rule - x0 must always read as zero no matter what gets written to it
added the check directly in the write function so nothing downstream has to worry about it

### DAY 14 :
stopping here per the plan - nothing executes yet, this step is only state management
next up is wiring memory.c testing before moving into actual instruction execution

DAY 15 :

went back and cleared the leftover checkbox from step 3 - isolated testing of memory.c poked known hardcoded values directly into the memory array, read them back with the 8/16/32-bit read functions confirmed the little-endian byte assembly actually matches what i expect before trusting it with real instructions

DAY 16 :

implemented fetch - grabs the raw 32-bit instruction word from memory at pc, reused the existing 32-bit read function instead of duplicating logic also handled pc increment right after, +4 every cycle since instructions are word aligned, gets overridden later by jumps/branches

DAY 17 :

wrote decoder.c extracted opcode/rd/funct3/rs1/rs2/funct7 with bitwise shift-and-mask, then did immediate assignment for all five formats (I/S/B/U/J), each one reconstructed bit slice by bit slice with sign extension applied at the end

DAY 18 :

wired fetch, decode, and execute into one full cycle decoder fully implemented now, all six formats identified and immediates coming out correctly cpu can now actually fetch -> decode -> execute in a loop, next step is filling out the execute logic per instruction

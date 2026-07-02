# PERSONAL NOTES 
reference material : KU LEUVEN  


## SECTION 1 :

Covers basic architecture knowledge and fetch-decode-execute cycle. 
Understand the speeed difference between SSD/HDD , RAM , caches and registers
this list is in ascending order in terms of hardware speed with SSD/HDD being the slowest and registers the fastest

machine code is binary and the assembly code is just the mnemonic form

Different processors can execute different instructions. The list of instructions a given CPU can execute is defined in the instruction set architecture (ISA).

this project will use RV32I (32 bit integer) ISA...specifying 32 32 bit registers and 47 instructions which are also encoded as 32 bit words.


However since we have larger RAM sizes now we need more and bigger registers (64 bit).
since registers hold data that addresses the data in ram, hence if we have more addressable memory we need larger registers. 

pseudo instructions basically dont have direct machine code representation so they translate to either two consecutive machine code lines or switched out for another one.

variables in C are stored in memory locations...to perform operations on these values u laod them in to registers first using LW.....& operator gives the address of a variable ; used in scanf

pointers used to store adresses pointing to certain values

use double pointers to store addresses of pointer variables

### MEMORY SECTIONS : 

sometimes we do not want to store in registers and need to store in memory 

#### .text SECTION:

program code is stored in this section

we can also use labels like function names to point to certain instructions
for eg: 
**main** : 
    **assembly code**

main is special...RARS knows to start execution from this point
use .globl main to allow other programs to start exectiion from main

#### .data SECTION:

used to store variables 

**.data**
    **a: .word 5**

There is a weaker notion of data types in assembly. For integers, we will usually reserve a word (32 bits) of memory, which corresponds to the size of int in C in most cases

We also use a label (a:) to give a name to our word in memory, otherwise it would be difficult to refer back to it.

we chose to give our variable a the initial value 5.

## SECTION 2 :
functions in C dont exist in assembly, theyre just labels + jumps + registers, held together by convention not hardware
struct fields are laid out consecutively in memory but compiler can add padding between fields for alignment/optimization
pointer to struct : use -> instead of (*ptr).field, same thing just cleaner syntax
passing a struct by value copies it, so changes inside the function dont persist...need to pass a pointer if you want the original modified
arrays are just contiguous memory, the array name itself decays to a pointer to the first element
arr[i] is really just syntactic sugar for *(arr + i)....pointer arithmetic auto-scales by the size of the type (int* += 1 actually moves 4 bytes)
arrays dont know their own length, no built in bounds checking, need a sentinel value or separate length variable to know where they end
strings in C are just char arrays terminated with a null byte \0......this null byte is the sentinel that lets strlen/loops know where to stop
in assembly, arrays are declared with comma separated values in .data, eg:
```
.data
    array: .word 1, 2, 3, 4
```
strings use .string / .asciz directive, adds the null terminator automatically
to iterate arrays in assembly u just walk the base address with an offset (word = +4 each step, byte/char = +1 each step)
lbu = load byte unsigned, used for reading individual characters out of a string
this section was mostly about realizing there is no "array type" or "string type" in assembly, its all just memory + discipline about how far to walk
 
## SECTION 3 :
biggest lesson : assembly has no native function concept, its all built from labels/jumps/registers + a set of AGREED UPON rules called calling conventions
why conventions matter : if caller uses t0 for a value and callee also stomps on t0 without warning, the value is just gone. conventions exist so every function (including ones written by someone else, or the compiler) can interoperate safely
register roles :
- x0 (zero) - hardwired zero
- ra (x1) - return address
- sp (x2) - stack pointer
- t0-t6 - temporary / CALLER-saved (function is free to overwrite these, so caller must save them first if it needs them after the call)
- s0-s11 - saved / CALLEE-saved (function must restore these before returning if it uses them)
- a0-a7 - argument registers, also doubles as a0/a1 for return values
stack basics : LIFO structure, grows DOWNWARDS in RISC-V (decrement sp to allocate, increment to free)
push = addi sp, sp, -4 then sw reg, 0(sp)
pop = lw reg, 0(sp) then addi sp, sp, 4
sp should always point at the last pushed value, not the empty slot below it
why downward growth : pure convention, but it lets stack and heap grow toward each other from opposite ends of memory and share the same block without needing to predict sizes upfront
memory layout bottom to top : reserved -> .text (instructions) -> .data (static vars) -> heap (grows up) -> stack (grows down)
jal (jump and link) = jumps to a label AND automatically stores the return address (pc+4) into ra....this is what makes "returning" possible
full calling convention checklist for calling a function :
1. push any caller-saved regs you still need after the call
2. put args into a0-a7 (spill extras onto the stack if you run out)
3. jal to the function (this sets ra)
inside the called function :
1. back up ra on the stack if youre going to call another function yourself (nested calls overwrite ra)
2. back up any callee-saved regs (s0-s11) you plan to use
3. do the actual work
4. put return value in a0 (a1 too if needed)
5. restore callee-saved regs + ra
6. ret (jumps back to whatever is in ra)
recursion = literally just a function calling itself, same stack discipline applies, each call gets its own stack frame (own copy of ra + saved regs)
tail recursion is special : if the recursive call is the LAST thing the function does (nothing happens after it returns), you dont need jal/ret at all - just overwrite the args and use a plain jump (j), since you're not adding a new stack frame at all
non-tail recursion (like factorial, where you still need to multiply AFTER the recursive call returns) can't use this trick, has to build up a full stack frame per call

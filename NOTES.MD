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
.section .text
.globl _start

_start:
    la sp, _stack_top
    call main
    ebreak

.section .bss
.align 4
.space 65536        # 64KB stack
_stack_top:

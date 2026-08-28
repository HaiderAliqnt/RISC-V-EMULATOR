.section .text
.globl _start
_start:
    la sp, _stack_top

    # Zero .bss: for (p = __bss_start; p < __bss_end; p++) *p = 0;
    la t0, __bss_start
    la t1, __bss_end
zero_bss_loop:
    bge t0, t1, zero_bss_done
    sb zero, 0(t0)
    addi t0, t0, 1
    j zero_bss_loop
zero_bss_done:

    call main
    ebreak

.section .bss
.align 4
.space 65536        # 64KB stack
_stack_top:

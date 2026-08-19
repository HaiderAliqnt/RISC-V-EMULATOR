#include <stdint.h>

static void *sbrk(int increment) {
    register int   a0 __asm__("a0") = increment;
    register int   a7 __asm__("a7") = 214;
    __asm__ volatile ("ecall" : "+r"(a0) : "r"(a7));
    return (void *)(uintptr_t)a0;
}

void _start() {
    volatile char *uart = (volatile char *)0x10000000;

    void *heap1 = sbrk(0);     // get current heap pointer
    void *heap2 = sbrk(1024);  // allocate 1024 bytes
    void *heap3 = sbrk(0);     // get new heap pointer

    // heap2 should equal heap1
    // heap3 should equal heap1 + 1024
    if (heap2 == heap1 && heap3 == (void *)((char *)heap1 + 1024)) {
        *uart = 'S';  // sbrk working
    } else {
        *uart = 'F';  // sbrk broken
    }

    __asm__("ebreak");
}

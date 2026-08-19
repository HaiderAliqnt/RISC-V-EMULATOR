// timer_test.c
#include <stdint.h>
void _start() {
    volatile uint32_t *timer = (volatile uint32_t *)0x30000000;
    uint32_t t1 = *timer;

    // busy wait loop
    for (volatile int i = 0; i < 100000; i++);

    uint32_t t2 = *timer;

    // t2 should be greater than t1
    volatile char *uart = (volatile char *)0x10000000;
    if (t2 > t1) {
        *uart = 'T';  // timer working
    } else {
        *uart = 'F';  // timer broken
    }

    __asm__("ebreak");
}

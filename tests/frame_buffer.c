#include <stdint.h>
#define FB_BASE    0x20000000
#define VSYNC_BASE 0x20010000
#define WIDTH      320
#define HEIGHT     200

void _start() {
    volatile uint8_t  *fb    = (volatile uint8_t  *)FB_BASE;
    volatile uint32_t *vsync = (volatile uint32_t *)VSYNC_BASE;

    // fill screen with color index 5
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        fb[i] = 5;
    }

    // trigger frame render
    *vsync = 1;

    __asm__("ebreak");
}

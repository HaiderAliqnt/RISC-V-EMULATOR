#include <stdint.h>
#define FB_BASE    0x20000000
#define VSYNC_BASE 0x20010000
#define WIDTH      320
#define HEIGHT     200

int main(void) {
    volatile uint8_t  *fb    = (volatile uint8_t  *)FB_BASE;
    volatile uint32_t *vsync = (volatile uint32_t *)VSYNC_BASE;

    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        fb[i] = 5;
    }

    *vsync = 1;
    volatile int x = 0;

    while (1) {
        x++;
    }

    return 0;
}

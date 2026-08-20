#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "../include/memory.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

static SDL_Window   *window   = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture  *texture  = NULL;
static uint32_t      palette[256];  // DOOM color palette
static uint8_t       framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
static uint8_t *heap = NULL;
static uint8_t *mem = NULL;


static void render_frame(void) {
    uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        pixels[i] = palette[framebuffer[i]];
    }
    SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

static int handle_mmio_write(uint32_t address, uint32_t value) {




    if (address == UART_BASE) {
        putchar((char)(value & 0xFF));  // UART output... prints the character
        fflush(stdout);
        return 1;  // handled
    }

    // Framebuffer — each byte is a palette index
    if (address >= FRAMEBUFFER_BASE &&
        address <  FRAMEBUFFER_BASE + FRAMEBUFFER_SIZE) {
        // printf("FB write OK: 0x%08X\n", address);
        framebuffer[address - FRAMEBUFFER_BASE] = (uint8_t)(value & 0xFF);
        return 1;
    }

        // Palette write — DOOM writes palette data to a specific address
        // 256 colors x 3 bytes (RGB) = 768 bytes
    if (address >= PALETTE_BASE && address < PALETTE_BASE + 768) {
            int32_t index = address - PALETTE_BASE;  // byte offset into palette data
                uint32_t color_index = index / 3;         // which color (0-255)
                uint32_t channel     = index % 3;         // 0=R, 1=G, 2=B

                static uint8_t rgb[256][3];               // temporary storage for RGB bytes
                rgb[color_index][channel] = (uint8_t)(value & 0xFF);

                // once all 3 channels are written, build the ARGB value
                if (channel == 2) {
                    palette[color_index] = (0xFF      << 24) |
                        (rgb[color_index][0] << 16) |
                        (rgb[color_index][1] << 8)  |
                        (rgb[color_index][2]);
                }
                return 1;
    }
    if (address == VSYNC_BASE) {
        printf("VSYNC hit\n");
        render_frame();
        // SDL_Event event;
        // while (SDL_PollEvent(&event)) {
        //     if (event.type == SDL_QUIT) {
        //             exit(0);
        //     }
        // }
        return 1;
    }
    // printf("MMIO miss: 0x%08X\n", address);
    return 0;  // not an MMIO address
}

void init_default_palette(void) {
    memset(palette, 0, sizeof(palette));
    palette[0] = 0xFF000000;  // black
    palette[1] = 0xFFFFFFFF;  // white
    palette[2] = 0xFFFF0000;  // red
    palette[3] = 0xFF00FF00;  // green
    palette[4] = 0xFF0000FF;  // blue
    palette[5] = 0xFFFF00FF;  // magenta — this is what your test writes
}

static void check_bounds(uint32_t address , uint32_t size){

    if (address + size <= MEMORY_SIZE) return;
    if (address >= HEAP_BASE && address + size <= HEAP_BASE + HEAP_SIZE) return;
    fprintf(stderr, "Memory access out of bounds: 0x%08X\n", address);
    exit(1);
}

void memory_init(void){

    mem = (uint8_t *)malloc(MEMORY_SIZE);
    heap = (uint8_t *)malloc(HEAP_SIZE);
    if(mem == NULL || heap == NULL){
        fprintf(stderr , "Failed to allocate memory\n");
        exit(1);
    }
    memset(mem , 0 , MEMORY_SIZE);
    memset(heap, 0, HEAP_SIZE);
    printf("Memory allocated succesfully"); //zero everything out

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "RISC-V Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH * 2,   // scale up 2x so it is not tiny
        SCREEN_HEIGHT * 2,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );

    init_default_palette();
}

void memory_free(void){

    if (texture)  SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    SDL_Quit();
    if(mem!=NULL){
        free(mem);
        mem = NULL;
    }
    printf("Nothing in memory to free");
}

void memory_zero(uint32_t address, uint32_t size) {
    check_bounds(address, size);
    memset(mem + address, 0, size);
}

uint8_t memory_read8(uint32_t address){

    if (address >= HEAP_BASE && address < HEAP_BASE + HEAP_SIZE) {
            return heap[address - HEAP_BASE];
        }

    if (address == TIMER_BASE) {
        return (uint32_t)SDL_GetTicks();  // milliseconds since SDL_Init
    }

    if (address >= FRAMEBUFFER_BASE &&
        address <  FRAMEBUFFER_BASE + FRAMEBUFFER_SIZE) {
        return framebuffer[address - FRAMEBUFFER_BASE];
    }

    check_bounds(address , 1);
    return mem[address];

}

uint16_t memory_read16(uint32_t address){

    if (address >= HEAP_BASE && address < HEAP_BASE + HEAP_SIZE) {
            return heap[address - HEAP_BASE];
    }

    if (address == TIMER_BASE) {
        return (uint32_t)SDL_GetTicks();  // milliseconds since SDL_Init
    }
    if (address >= FRAMEBUFFER_BASE &&
        address <  FRAMEBUFFER_BASE + FRAMEBUFFER_SIZE) {
        return framebuffer[address - FRAMEBUFFER_BASE];
    }
    check_bounds(address, 2);
    return (uint16_t)(mem[address])|
    (uint16_t)(mem[address+1] << 8);

}

uint32_t memory_read32(uint32_t address){

    if (address >= HEAP_BASE && address < HEAP_BASE + HEAP_SIZE) {
            return heap[address - HEAP_BASE];
    }

    if (address == TIMER_BASE) {
        return (uint32_t)SDL_GetTicks();  // milliseconds since SDL_Init
    }
    if (address >= FRAMEBUFFER_BASE &&
        address <  FRAMEBUFFER_BASE + FRAMEBUFFER_SIZE) {
        return framebuffer[address - FRAMEBUFFER_BASE];
    }
    check_bounds(address, 4);
    return  (uint32_t)(mem[address])         |
            (uint32_t)(mem[address+1]) << 8  |
            (uint32_t)(mem[address+2]) << 16 |
            (uint32_t)(mem[address+3]) << 24;

}

void  memory_write8(uint32_t address, uint8_t value){

    // printf("DEBUG write8: address=0x%08X value=0x%02X\n", address, value);
    if (address >= HEAP_BASE && address < HEAP_BASE + HEAP_SIZE) {
        heap[address - HEAP_BASE] = value;
        return;
    }
    if (handle_mmio_write(address, value)) return;
    check_bounds(address, 1);
    mem[address] = value;

}

void memory_write16(uint32_t address, uint16_t value){

    if (address >= HEAP_BASE && address < HEAP_BASE + HEAP_SIZE) {
        heap[address - HEAP_BASE] = value;
        return;
    }

    if (handle_mmio_write(address, value)) return;
    check_bounds(address , 2);
    mem[address] = (uint8_t)(value & 0XFF);
    mem[address+1] = (uint8_t)((value>>8) & 0XFF);

}

void memory_write32(uint32_t address, uint32_t value){

    // printf("DEBUG write8: address=0x%08X value=0x%02X\n", address, value);

    if (address >= HEAP_BASE && address < HEAP_BASE + HEAP_SIZE) {
        heap[address - HEAP_BASE] = value;
        return;
    }
    if (handle_mmio_write(address, value)) return;
    check_bounds(address , 4);
    mem[address] = (uint8_t)(value & 0XFF);
    mem[address+1] = (uint8_t)((value>>8) & 0XFF);
    mem[address+2] = (uint8_t)((value>>16) & 0XFF);
    mem[address+3] = (uint8_t)((value>>24) & 0XFF);

}

void memory_load(uint32_t address, uint8_t *data, uint32_t size){

    check_bounds(address, size);
    memcpy(mem+address , data, size);

}

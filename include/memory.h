#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define MEMORY_SIZE 0X10000000 //256MB

#define UART_BASE 0X10000000 //Mem mapped I/O addr
#define TIMER_BASE 0x30000000
#define FRAMEBUFFER_BASE  0x20000000
#define SCREEN_WIDTH      320
#define SCREEN_HEIGHT     200
#define FRAMEBUFFER_SIZE  (SCREEN_WIDTH * SCREEN_HEIGHT)
#define VSYNC_BASE        0x20010000
#define PALETTE_BASE      0x20020000
#define HEAP_BASE 0x40000000
#define HEAP_SIZE 0x04000000

void memory_init(void);
void memory_free(void);
void memory_zero(uint32_t address, uint32_t size);

uint8_t memory_read8(uint32_t address);
uint16_t memory_read16(uint32_t address);
uint32_t memory_read32(uint32_t address);

void memory_write8(uint32_t address , uint8_t value );
void memory_write16(uint32_t address , uint16_t value );
void memory_write32(uint32_t address , uint32_t value );


void memory_load(uint32_t address, uint8_t *data, uint32_t size);



#endif

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define MEMORY_SIZE 0X10000000 //256MB

#define UART_BASE 0X10000000 //Mem mapped I/O addr

void memory_init(void);
void memory_free(void);

uint8_t memory_read8(uint32_t address);
uint16_t memory_read16(uint32_t address);
uint32_t memory_read32(uint32_t address);

void memory_write8(uint32_t address , uint8_t value );
void memory_write16(uint32_t address , uint16_t value );
void memory_write32(uint32_t address , uint32_t value );


void memory_load(uint32_t address, uint8_t *data, uint32_t size);



#endif

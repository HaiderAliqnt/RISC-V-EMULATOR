#include <stdio.h>
#include <stdint.h>
#include "../include/memory.h"


int main(){

    memory_init();

    printf("\n Testing the memory module...\n");



    memory_write8(0x00, 0xAB);
    memory_write8(0x01, 0xFF);
    memory_write8(0x10, 0x42);

    printf("[8-bit]\n");
    printf("addr 0x00 expected=0xAB got=0x%02X %s\n", memory_read8(0x00), memory_read8(0x00) == 0xAB ? "PASS" : "FAIL");
    printf("addr 0x01 expected=0xFF got=0x%02X %s\n", memory_read8(0x01), memory_read8(0x01) == 0xFF ? "PASS" : "FAIL");
    printf("addr 0x10 expected=0x42 got=0x%02X %s\n", memory_read8(0x10), memory_read8(0x10) == 0x42 ? "PASS" : "FAIL");

    memory_write16(0x100,0x1234);
    memory_write16(0x200, 0xBEEF);

    printf("\n[16-bit]\n");
    printf("addr 0x100 expected=0x1234 got=0x%04X %s\n", memory_read16(0x100), memory_read16(0x100) == 0x1234 ? "PASS" : "FAIL");
    printf("addr 0x200 expected=0xBEEF got=0x%04X %s\n", memory_read16(0x200), memory_read16(0x200) == 0xBEEF ? "PASS" : "FAIL");

    memory_write32(0x1000, 0x12345678);
    memory_write32(0x2000, 0xDEADBEEF);
    memory_write32(0x3000, 0x00000001);
    memory_write32(0x4000, 0x80000000);

    printf("\n[32-bit]\n");
    printf("addr 0x1000 expected=0x12345678 got=0x%08X %s\n", memory_read32(0x1000), memory_read32(0x1000) == 0x12345678 ? "PASS" : "FAIL");
    printf("addr 0x2000 expected=0xDEADBEEF got=0x%08X %s\n", memory_read32(0x2000), memory_read32(0x2000) == 0xDEADBEEF ? "PASS" : "FAIL");
    printf("addr 0x3000 expected=0x00000001 got=0x%08X %s\n", memory_read32(0x3000), memory_read32(0x3000) == 0x00000001 ? "PASS" : "FAIL");
    printf("addr 0x4000 expected=0x80000000 got=0x%08X %s\n", memory_read32(0x4000), memory_read32(0x4000) == 0x80000000 ? "PASS" : "FAIL");

    //functions to check little endian layout
    memory_write32(0x5000, 0x12345678);
    printf("\n[little endian layout]\n");
    printf("addr 0x5000 expected=0x78 got=0x%02X %s\n", memory_read8(0x5000), memory_read8(0x5000) == 0x78 ? "PASS" : "FAIL");
    printf("addr 0x5001 expected=0x56 got=0x%02X %s\n", memory_read8(0x5001), memory_read8(0x5001) == 0x56 ? "PASS" : "FAIL");
    printf("addr 0x5002 expected=0x34 got=0x%02X %s\n", memory_read8(0x5002), memory_read8(0x5002) == 0x34 ? "PASS" : "FAIL");
    printf("addr 0x5003 expected=0x12 got=0x%02X %s\n", memory_read8(0x5003), memory_read8(0x5003) == 0x12 ? "PASS" : "FAIL");

    // memory_load bulk test
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    memory_load(0x6000, data, sizeof(data));

    printf("\n[bulk load]\n");
    printf("addr 0x6000 expected=0x01 got=0x%02X %s\n", memory_read8(0x6000), memory_read8(0x6000) == 0x01 ? "PASS" : "FAIL");
    printf("addr 0x6001 expected=0x02 got=0x%02X %s\n", memory_read8(0x6001), memory_read8(0x6001) == 0x02 ? "PASS" : "FAIL");
    printf("addr 0x6004 expected=0x05 got=0x%02X %s\n", memory_read8(0x6004), memory_read8(0x6004) == 0x05 ? "PASS" : "FAIL");

    // zero init check
    printf("\n[zero init]\n");
    printf("addr 0x9999 expected=0x00 got=0x%02X %s\n", memory_read8(0x9999), memory_read8(0x9999) == 0x00 ? "PASS" : "FAIL");

    // bounds check
    printf("\n[bounds check] -- next line should trigger out of bounds error\n");
    memory_read8(0x10000000);

    memory_free();
    return 0;

}

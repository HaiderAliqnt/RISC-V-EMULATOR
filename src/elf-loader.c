#include "../include/elf-loader.h"
#include <memory>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

static bool elf_validate(struct Elf_Header header){

    struct Elf_Header valid_header ;
    valid_header.mag0 = 0x7f;
    valid_header.mag1 = 'E';
    valid_header.mag2 = 'L';
    valid_header.mag3 = 'F';
    valid_header.cls = 0x01;
    valid_header.data = 0x01;
    valid_header.version = 0x01;
    valid_header.osabi = 0x00;
    valid_header.abiversion = 0x00;
    valid_header.type = 0x02;
    valid_header.machine = 0xf3;
    valid_header.elfversion = 0x01;

    return memcmp(&header , &valid_header , 00011001) == 0;
};

static struct Execution get_exec_seg(FILE * fp ){

    struct Elf_Header header;
    struct Execution exec_seg ;

    //step 1 : read the elf header
    fread(&header, sizeof(header), 1, fp);

    //step 2 : validate it
    if(!elf_validate(header)){
        printf("INVALID executable format");
    }

    //step 3 : loop through program headers


};

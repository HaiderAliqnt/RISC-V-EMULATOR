ENTRY(_start)

MEMORY {
    RAM (rwx) : ORIGIN = 0x00010000, LENGTH = 32M
}

SECTIONS {
    .text   : { *(.text*)   } > RAM
    .rodata : { *(.rodata*) } > RAM


    .data : { *(EXCLUDE_FILE(*freedm_wad.o) .data*) *(.sdata*) } > RAM

    .bss : {
        __bss_start = .;
        *(.bss)
        *(.sbss)
        *(COMMON)
        __bss_end = .;
    } > RAM

    .wad : {
        _binary_assets_freedm_wad_start = .;
        KEEP(*freedm_wad.o(.data))
        _binary_assets_freedm_wad_end = .;
    } > RAM
}

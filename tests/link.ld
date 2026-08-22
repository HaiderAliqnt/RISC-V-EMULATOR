ENTRY(_start)

MEMORY {
    RAM (rwx) : ORIGIN = 0x00010000, LENGTH = 32M
}

SECTIONS {
    .text   : { *(.text*)   } > RAM
    .rodata : { *(.rodata*) } > RAM
    .data   : { *(.data*)   } > RAM
    .bss    : { *(.bss*)    } > RAM
}
.wad : {
    _binary_doom1_wad_start = .;
    KEEP(*(.wad))
    _binary_doom1_wad_end = .;
} > RAM

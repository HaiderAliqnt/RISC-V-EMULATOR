ENTRY(_start)

MEMORY {
    RAM (rwx) : ORIGIN = 0x00010000, LENGTH = 1M
}

SECTIONS {
    .text : {
        *(.text)
    } > RAM
}

extern int main(void);

void _start(void) {
    main();
    __asm__("ebreak");
}

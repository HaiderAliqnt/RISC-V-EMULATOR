// write_test.c
static void sys_write(int fd, const char *buf, int count) {
    register int        a0 __asm__("a0") = fd;
    register const char *a1 __asm__("a1") = buf;
    register int        a2 __asm__("a2") = count;
    register int        a7 __asm__("a7") = 64;
    __asm__ volatile ("ecall" : : "r"(a0), "r"(a1), "r"(a2), "r"(a7));
}

void _start() {
    const char *msg = "syscall write works\n";
    int len = 0;
    while (msg[len]) len++;
    sys_write(1, msg, len);
    __asm__("ebreak");
}

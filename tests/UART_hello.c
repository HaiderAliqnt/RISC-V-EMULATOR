void uart_putchar(char c) {
    volatile char *uart = (volatile char *)0x10000000;
    *uart = c;
}

void uart_print(const char *str) {
    while (*str) {
        uart_putchar(*str++);
    }
}

int main() {
    uart_print("Hello World\n");
    return 0;
}

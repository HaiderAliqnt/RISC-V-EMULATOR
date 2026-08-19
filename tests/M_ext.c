// mext_test.c
void _start() {
    volatile char *uart = (volatile char *)0x10000000;

    int a = 6;
    int b = 7;
    int mul_result = a * b;   // should use MUL    = 42
    int div_result = a / b;   // should use DIV    = 0
    int rem_result = 42 % b;  // should use REM    = 0

    if (mul_result == 42) *uart = 'M';  // MUL works
    else                  *uart = 'F';

    if (div_result == 0)  *uart = 'D';  // DIV works
    else                  *uart = 'F';

    if (rem_result == 0)  *uart = 'R';  // REM works
    else                  *uart = 'F';

    __asm__("ebreak");
}

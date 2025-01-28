void puts(const char *s);

void main()
{
    puts("\nQEMU Ramfb Driver Demo\n");
}

#define UART_BASE 0x10000000
#define UART_THR  (unsigned char *)(UART_BASE + 0x0)
#define UART_LSR  (unsigned char *)(UART_BASE + 0x5)

static void send(char c)
{
    while ((*UART_LSR & 0x20) == 0)
        ;
    *UART_THR = c;
}

void puts(const char *s)
{
    while (*s) {
        if (*s == '\n')
            send('\r');
        send(*s++);
    }
}

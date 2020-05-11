#include <detpic32.h>

int main(void) {
    // Configure UART1
    // BaudRate Generator
    U1MODEbits.BRGH = 0;
        // 1 = High-Speed mode – 4x baud clock enabled
        // 0 = Standard Speed mode – 16x baud clock enabled
    unsigned int baudrate = 115200;
    U1BRG = (PBCLK + 8 * baudrate) / (16 * baudrate) - 1;

    // Number of data bits, parity and number of stop bits
    U1MODEbits.STSEL = 0;
        // 1 = 2 Stop bits
        // 0 = 1 Stop bit
    U1MODEbits.PDSEL = 0;
        // 11 = 9-bit data, no parity
        // 10 = 8-bit data, odd parity
        // 01 = 8-bit data, even parity
        // 00 = 8-bit data, no parity
    
    // Enable the trasmitter and receiver modules
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;

    // Enable UART1
    U1MODEbits.ON = 1;

    return 0;
}
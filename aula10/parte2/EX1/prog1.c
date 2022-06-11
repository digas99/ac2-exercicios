#include <detpic32.h>

void delay(unsigned int );
void putc1(char );

int main(void) {
    // UART1 conf
    unsigned int baudrate = 115200;
    U1BRG = ((PBCLK+8*baudrate) / (16*baudrate)) - 1; 
    U1MODEbits.BRGH = 0;
    U1MODEbits.PDSEL = 0b00;
    U1MODEbits.STSEL = 0;
    U1STAbits.URXEN = 1;    
    U1STAbits.UTXEN = 1;
    U1MODEbits.ON = 1;

    while(1) {
        putc1(0x5A);
        delay(10);
    }

    return 0;
}

void putc1(char byte) {
    while(U1STAbits.UTXBF == 1);
    U1TXREG = byte;
}

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < (PBCLK/1000)*ms);
}
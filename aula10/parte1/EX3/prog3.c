#include <detpic32.h>

void delay(unsigned int );
void putc(char );
void putstr(char *);

int main(void) {
    // UART2 conf
    unsigned int baudrate = 115200;
    U2BRG = ((PBCLK+8*baudrate) / (16*baudrate)) - 1; 
    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL = 0b00;
    U2MODEbits.STSEL = 0;
    U2STAbits.URXEN = 1;    
    U2STAbits.UTXEN = 1;
    U2MODEbits.ON = 1;

    while(1) {
        putstr("Put string\n");
        delay(1000);
    }

    return 0;
}

void putc(char byte) {
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte;
}

void putstr(char *str) {
    while(*str != '\0') {
        putc(*str);
        str++;
    }
}

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < (PBCLK/1000)*ms);
}
#include <detpic32.h>
#include "functions.h"

void UART1config(int , char , int );
void putc(char );
void puts(char *);

int main(void) {
    // Configure UART1
    //UART1config(600,'N',1);
    //UART1config(1200,'O',2);
    //UART1config(9600,'E',1);
    //UART1config(19200,'N',2);
    UART1config(115200,'E',1);
    while(1) {
        puts("String de teste\n");
        delay(1000);
    }
    return 0;
}

void UART1config(int baudrate, char parity, int stop_bits) {
    U1MODEbits.BRGH = 0;
    U1BRG = (baudrate >= 600 && baudrate <= 115200) ? (PBCLK + 8 * baudrate) / (16 * baudrate) - 1 : (PBCLK + 8 * 115200) / (16 * 115200) - 1;
    U1MODEbits.STSEL = stop_bits == 2 ? 1 : 0;
    switch (parity) {
        case 'e':
        case 'E':
            U1MODEbits.PDSEL = 1;
            break;
        case 'o':
        case 'O':
            U1MODEbits.PDSEL = 10;
            break;
        default:
            U1MODEbits.PDSEL = 0;
    }
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;
}

void putc(char byte2send) {
    while (U1STAbits.UTXBF == 1);
    U1TXREG = byte2send;
}

void puts(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        putc(str[i++]);
    }
}
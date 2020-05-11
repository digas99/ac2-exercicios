#include <detpic32.h>

void UART1config(int , char , int );
void putc(char );
void puts(char *);

int main(void) {
    // Configure UART1
    UART1config(115200,'N',1);
    TRISBbits.TRISB6 = 0; // configure RB6 as output
    while(1) {
        resetCoreTimer();
        while(U1STAbits.TRMT == 0);
        LATBbits.LATB6 = 1; // set RB6
        //puts("12345");
        //puts("123456789");
        //puts("123456789A");
        puts("123456789AB");
        LATBbits.LATB6 = 0; // reset RB6
        printStr("\nTime: ");
        printInt10(readCoreTimer()); // because oscilloscope is missing at the moment
        printStr("ns\n");
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
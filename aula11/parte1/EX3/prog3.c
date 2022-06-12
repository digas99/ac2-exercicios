#include <detpic32.h>

void configUart2(unsigned int , char , unsigned int );
void putc(char );

int main(void) {
    configUart2(115200, 'N', 1);
     
    IEC1bits.U2TXIE = 0;
    IEC1bits.U2RXIE = 1;
    IPC8bits.U2IP = 2;
    IFS1bits.U2RXIF = 0;
    U2STAbits.URXISEL = 0b00;

    EnableInterrupts();

    TRISCbits.TRISC14 = 0;

    while(1);

    return 0;
}

char value;

void _int_(24) isr_uart2(void) {
    if (IFS1bits.U2RXIF == 1) {
        value = U2RXREG;

        if (value == 'T')
            LATCbits.LATC14 = 1;

        if (value == 't')
            LATCbits.LATC14 = 0;

        putc(value);
        IFS1bits.U2RXIF = 0;
    }
}

void putc(char byte) {
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte;
}

void configUart2(unsigned int baud, char parity, unsigned int stopbits) {
    baud = baud == 600 || baud == 115200 ? baud : 115200;
    U2BRG = ((PBCLK+8*baud) / (16*baud)) - 1;
    U2MODEbits.BRGH = 0;
    
    switch(parity) {
        case 'E':
            U2MODEbits.PDSEL = 0b01;
            break;
        case 'O':
            U2MODEbits.PDSEL = 0b10;
            break;
        default:
            U2MODEbits.PDSEL = 0b00;
    }
    U2MODEbits.STSEL = stopbits == 1 || stopbits == 2 ? stopbits : 1;

    U2STAbits.URXEN = 1;    
    U2STAbits.UTXEN = 1;
    U2MODEbits.ON = 1;
}
#include <detpic32.h>

void putc(char );
void putstr(char *);

int main(void) {
    // config UART2
    int baudrate = 9600;
    U2BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) -1;
    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL = 0b10; // 8bits, odd parity
    U2MODEbits.STSEL = 1; // 2 stop bits
    U2STAbits.URXEN = 1; // enable reception
    U2STAbits.URXEN = 1; // enable transmition
    U2MODEbits.ON = 1;

    // UART2 Rx interrupts
    U2STAbits.URXISEL = 0b00;
    IEC1bits.U2RXIE = 1;
    IPC8bits.U2IP = 2;
    IFS1bits.U2RXIF = 0;

    TRISB = TRISB | 0x0F; // set RB3-0 as inputs
    TRISEbits.TRISE4 = 0; // set RE4 as output

    LATEbits.LATE4 = 1;

    EnableInterrupts();

    while(1);

    return 0;
}

void _int_(32) isr_uart2(void) {
    if (IFS1bits.U2RXIF == 1) {
        switch(U2RXREG) {
            case 'P':
                putstr("DipSwitch=");
                putc(PORTB & 0x0F);
                putc('\n');
                break;
            case 'T':
                LATEbits.LATE4 = !LATEbits.LATE4;
                break;
        }

        IFS1bits.U2RXIF = 0;
    }
}

void putc(char c) {
    while(U2STAbits.UTXBF == 1); // wait until buffer not full
    U2TXREG = c;
}

void putstr(char *s) {
    while(*s != '\0') {
        putc(*s);
        s++;
    }
}
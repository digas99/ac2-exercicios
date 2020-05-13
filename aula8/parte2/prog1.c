#include <detpic32.h>

void UART1config(int , char , int );
void putc(char );

int main(void) {
    // Configure UART1
    UART1config(115200,'N',1);
    EnableInterrupts();
    while(1);
    return 0;
}

void _int_(24) isr_uart1(void) {
    putc(U1RXREG);
    // Clear UART1 rx interrupt flag
    IFS0bits.U1RXIF = 0;
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
    //RX interrupt enabled
    IPC6bits.U1IP = 2;		// configure priority of interrupts
	IEC0bits.U1RXIE = 1;	// enable interrupts
    U1MODEbits.ON = 1;
}

void putc(char byte2send) {
    while (U1STAbits.UTXBF == 1);
    U1TXREG = byte2send;
}
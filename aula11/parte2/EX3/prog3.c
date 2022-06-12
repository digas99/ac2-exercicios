#include <detpic32.h>

typedef struct {
    char mem[100];
    int nchar;
    int posrd;
} t_buf;

volatile t_buf txbuf;

void configUart2(unsigned int , char , unsigned int );
void putstrInt(char *);

int main(void) {
    configUart2(115200, 'N', 1);
    IEC1bits.U2TXIE = 0;
    IEC1bits.U2RXIE = 0;
    IFS1bits.U2TXIF = 0;   
    IPC8bits.U2IP = 2;
    U2STAbits.UTXISEL = 0b00;

    EnableInterrupts();

    txbuf.nchar = 0;

    while(1) {
        putstrInt("Test string which can be as long as you like as long as it is no longer than 100 characters\n");
    }

    return 0;
}

void putstrInt(char *s) {
    while(txbuf.nchar > 0);
    
    while(*s != '\0') {
        txbuf.mem[txbuf.nchar++] = *s;
        s++;
    }
    txbuf.posrd = 0;
    IEC1bits.U2TXIE = 1;
}

void _int_(32) isr_uart2(void) {
    if (IFS1bits.U2TXIF == 1) {
        if (txbuf.nchar-- > 0) {
            U2TXREG = txbuf.mem[txbuf.posrd++];
        }
        else {
            IEC1bits.U2TXIE = 0;
        }
        IFS1bits.U2TXIF = 0;   
    }
}

void configUart2(unsigned int baud, char parity, unsigned int stopbits) {
    baud = baud == 600 || baud == 115200 ? baud : 115200;
    U2BRG = ((PBCLK+8*baud) / (16*baud)) - 1;
    U2MODEbits.BRGH = 0;
    
    switch(parity) {
        case 'e':
            U2MODEbits.PDSEL = 0b01;
            break;
        case 'o':
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
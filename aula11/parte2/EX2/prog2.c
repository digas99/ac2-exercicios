#include <detpic32.h>

typedef struct {
    char mem[100];
    int nchar;
    int posrd;
} t_buf;

volatile t_buf txbuf;

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
        IFS1bits.U2TXIF == 0;   
    }
}
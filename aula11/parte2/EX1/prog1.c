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
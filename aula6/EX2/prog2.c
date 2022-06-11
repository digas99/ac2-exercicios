#include <detpic32.h>

int main(void) {
    volatile int aux;
    int n_samples = 1;

    TRISDbits.TRISD11 = 0;

    // setup A/d module
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = n_samples-1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    while(1) {
        AD1CON1bits.ASAM = 1;
        LATDbits.LATD11 = 1;
        while(IFS1bits.AD1IF == 0);
        LATDbits.LATD11 = 0;

        aux = ADC1BUF0;
        
        IFS1bits.AD1IF = 0;
    }

    return 0;
}
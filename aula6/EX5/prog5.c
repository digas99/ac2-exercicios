#include <detpic32.h>

int main(void) {
    int *p;
    int v, sum, avg, n_samples = 4; // number of consecutive samples

    // setup of A/D module
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = n_samples-1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    while(1) {
        sum = 0;

        AD1CON1bits.ASAM = 1;
        while(IFS1bits.AD1IF == 0);

        p = (int *)(&ADC1BUF0);
        for (; p <= (int *)(&ADC1BUFF); p+=4) {
            sum += *p;
        }
        avg = sum/n_samples;
        v = (avg*33+511)/1023;
        printStr("V = ");
        printInt(v, 10);
        putChar('\n');

        IFS1bits.AD1IF = 0;
    }   

    return 0; 
}
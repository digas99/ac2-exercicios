#include <detpic32.h>

#define K PBCLK/1000

void setupADC(unsigned int );
void send2displays(unsigned char );
void delay(unsigned int );
unsigned char toBcd(unsigned char );

int main(void) {
    int *p, i, v, sum, avg, n_samples = 4; // number of consecutive samples

    TRISB = TRISB & (0x80FF); // set displays as outputs
    TRISDbits.TRISD5 = 0; // set RD6-5 as outputs
    TRISDbits.TRISD6 = 0;

    setupADC(n_samples);

    while(1) {
        sum = 0, i = 0;

        AD1CON1bits.ASAM = 1;
        while(IFS1bits.AD1IF == 0);

        p = (int *)(&ADC1BUF0);
        for (; p <= (int *)(&ADC1BUF3); p+=4) {
            printInt(*p, 16 | 4 << 16);
            putChar(' ');
            sum += *p;
        }
        avg = sum/n_samples;
        v = (avg*33+511)/1023;
        printInt(v, 10);
        putChar('V');

        do {
            send2displays(toBcd(v));
            // refresh freq = 100Hz = 10ms
            delay(10);
        // measurement freq = 5Hz = 200ms = 20*10ms -> 200ms/10ms = 20
        } while(++i < (200/10));
        putChar('\n');
        
        IFS1bits.AD1IF = 0;
    }   

    return 0; 
}

void setupADC(unsigned int samples) {
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = samples-1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;
}

void send2displays(unsigned char value) {
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;
    
    if (!displayFlag) {
        LATDbits.LATD6 = 0;
        LATDbits.LATD5 = 1;
        LATB = (LATB & 0x80FF) | (display7Scodes[(value & 0x0F)] << 8);
    }
    else {
        LATDbits.LATD6 = 1;
        LATDbits.LATD5 = 0;
        LATB = (LATB & 0x80FF) | (display7Scodes[(value >> 4)] << 8);    
    }

    displayFlag = !displayFlag;
}

void delay(unsigned int ms) {
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}

unsigned char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}
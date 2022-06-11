#include <detpic32.h>

#define K PBCLK/1000

void setupADC(unsigned int );
void send2displays(unsigned char );
void delay(unsigned int );
unsigned char toBcd(unsigned char );

int samples = 8;
volatile int voltage;

int main(void) {
    int cnt=0;
    
    TRISB = TRISB & (0x80FF); // set displays as outputs
    TRISDbits.TRISD5 = 0; // set RD6-5 as outputs
    TRISDbits.TRISD6 = 0;

    // setup ADC
    setupADC(samples);

    // setup interrupt system
    IPC6bits.AD1IP = 2;
    IFS1bits.AD1IF = 0;
    IEC1bits.AD1IE = 1;
    EnableInterrupts();

    while(1) {
        if (cnt == 0) {
            // start A/D convertion
            AD1CON1bits.ASAM = 1;
        }
        send2displays(voltage);
        cnt = (cnt + 1) % 5;
        delay(10);
    }

    return 0;
}

// Vector Number for AD1 – ADC1 Convert Done: 27
// PIC32MX7xx_Family_Datasheet - page 75 - line 1 in the table
void _int_(27) isr_adc(void) {
    int *p, sum=0, avg;

    p = (int *)(&ADC1BUF0);
    for (; p <= (int *)(&ADC1BUF7); p+=4) {
        printInt(*p, 16 | 4 << 16);
        putChar(' ');
        sum += *p;
    }
    putChar('\n');
    avg = sum/samples;
    voltage = toBcd((avg*33+511)/1023);

    IFS1bits.AD1IF = 0;
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
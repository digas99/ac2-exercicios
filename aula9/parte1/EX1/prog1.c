#include <detpic32.h>

void setupADC(unsigned int );
void send2displays(unsigned char );
unsigned char toBcd(unsigned char );

volatile int voltage = 0;
int n_samples = 8;

int main(void) {
    TRISB = TRISB & (0x80FF); // set displays as outputs
    TRISDbits.TRISD5 = 0; // set RD6-5 as outputs
    TRISDbits.TRISD6 = 0;

    // Timer 1 config
    // Fout = 5Hz   PBCLK = 20MHz   PR1(max) = 65535
    // Kpresc = PBCLK/(65536*5) = 61
    // Kpresc = 64
    PR1 = PBCLK/64/5-1;
    T1CONbits.TCKPS = 2;
    TMR1 = 0;
    T1CONbits.TON = 1;

    // Timer 1 interrupts
    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    IFS0bits.T1IF = 0;

    // Timer 3 config
    // Fout = 100Hz   PBCLK = 20MHz   PR3(max) = 65535
    // Kpresc = PBCLK/(65536*100) = 3,05
    // Kpresc = 4
    PR3 = PBCLK/4/100-1;
    T3CONbits.TCKPS = 2;
    TMR3 = 0;
    T3CONbits.TON = 1;

    // Timer 3 interrupts
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0; 

    // ADC config
    setupADC(n_samples);

    // ADC interrupts
    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;
    IFS1bits.AD1IF = 0;

    EnableInterrupts();

    while(1);

    return 0; 
}

void _int_(4) isr_T1(void) {
    AD1CON1bits.ASAM = 1;

    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void) {
    send2displays(voltage);

    IFS0bits.T3IF = 0;
}

void _int_(27) isr_adc(void) {
    int *p, sum = 0, i, avg;

    p = (int *)(&ADC1BUF0);
    for (i = 0; i < n_samples; i++) {
        sum += p[i*4];
    }

    avg = sum/n_samples;
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

unsigned char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}
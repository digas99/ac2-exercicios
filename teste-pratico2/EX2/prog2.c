#include <detpic32.h>

void send2displays(unsigned char );
unsigned char toBcd(unsigned char );
void delay(unsigned int );

volatile unsigned int temperature;
unsigned int n_samples = 2;

int main(void) {
    TRISB = TRISB & 0x80FF; // set RB14-8 as outputs
    TRISD = TRISD & 0x9F; // set RD6-5 as outputs

    // config Timer 2
    // Kpresc = PBCLK/(65535*120) = 2,5 -> 4
    PR2 = PBCLK/120/4 - 1;
    T2CONbits.TCKPS = 2; // 1:4
    TMR2 = 0;
    T2CONbits.TON = 1;

    // Timer 2 interruptions
    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;

    // config ADC
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = n_samples-1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    EnableInterrupts();

    while(1) {
        AD1CON1bits.ASAM = 1;
        while(IFS1bits.AD1IF == 0);

        int i, sum=0, avg;
        for (i = 0; i < n_samples; i++) {
            sum += (&ADC1BUF0)[4*i];
        }

        avg = sum/n_samples;
        temperature = toBcd(((65*avg+511)/1023) + 20); // not sure about this values

        IFS1bits.AD1IF = 0;

        // 1/10Hz * 1000 = 100ms
        delay(1000);
    }

    return 0;
}

void _int_(8) isr_t2(void) {
    send2displays(temperature);

    IFS0bits.T2IF = 0;
}

void send2displays(unsigned char value) {
    static char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char flag = 0;

    if (!flag) {
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | (display7Scodes[value & 0x0F] << 8);
    }
    else {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | (display7Scodes[value >> 4] << 8);
    }

    flag = !flag;
}

unsigned char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < (PBCLK/1000) * ms);
}
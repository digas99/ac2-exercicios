#include <detpic32.h>

void setPWM(unsigned int );

int main(void) {
    // Timer 3 config
    // Kpresc = PBCLK/(65535*100) = 3 -> 4
    PR3 = PBCLK/4/100 - 1;
    T3CONbits.TCKPS = 2;
    TMR3 = 0;
    T2CONbits.TON = 1;
    OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 1;
    setPWM(25);
    OC1CONbits.ON = 1;

    TRISCbits.TRISC14 = 0;

    while (1) {
        LATCbits.LATC14 = PORTDbits.RD0;
    }
    
}

void setPWM(unsigned int dutyCycle) {
    if (dutyCycle >= 0 && dutyCycle <= 100)
        OC1RS = (PR3+1)*dutyCycle/100;
}
#include <detpic32.h>

int main(void) {
    // Timer 3 config
    // Kpresc = PBCLK/(65535*100) = 3 -> 4
    PR3 = PBCLK/4/100 - 1;
    T3CONbits.TCKPS = 2;
    TMR3 = 0;
    T2CONbits.TON = 1;
    OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 1;
    OC1RS = (PR3+1)*0,25;
    OC1CONbits.ON = 1;

    while(1);

    return 0;
}
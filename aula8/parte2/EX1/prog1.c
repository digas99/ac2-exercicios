#include <detpic32.h>

#define K PBCLK/1000

void delay(unsigned int );

int main(void) {
    TRISDbits.TRISD8 = 1; // set RD8 as input
    TRISEbits.TRISE0 = 0; // set RE0 as output

    while(1) {
        // polling for INT1 pulsator 
        while(IFS0bits.INT1IF == 0);
        
        LATEbits.LATE0 = 1;
        delay(3000);
        LATEbits.LATE0 = 0;

        IFS0bits.INT1IF = 0;
    }

    return 0;
}

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < K * ms);
}
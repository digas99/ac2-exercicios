#include <detpic32.h>

#define K PBCLK/1000

void delay(unsigned int );

int main(void) {
    TRISE = TRISE & 0xFFC0; // RE5-0 as outputs
    TRISBbits.TRISB2 = 1; // RB2 as input

    unsigned int value, ms, flag;
    int counter = 0;

    LATE = (LATE & 0xFFC0) | 0x01;
    while(1) {
        flag = PORTBbits.RB2;
        ms = 1/(flag ? 7 : 3)*1000;

        delay(ms);

        value = (PORTE & 0x003F) << 1;

        counter++;

        if (counter == 7) {
            value = 1;
            counter = 0;
        }

        LATE = (LATE & 0XFFC0) | value;
    }
}

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < K * ms);
}
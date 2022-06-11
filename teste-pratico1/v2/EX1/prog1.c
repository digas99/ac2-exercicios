#include <detpic32.h>

#define K PBCLK/1000

void delay(unsigned int );

int main(void) {
    // 0000 00XX
    TRISE = TRISE & 0xFF03; // set RE7-2 as outputs
    TRISBbits.TRISB0 = 1; // set RB2 and RB0 as inputs
    TRISBbits.TRISB2 = 1;

    int switches, value = 0x30; // value = 0011 0000
    while(1) {
        value = value > 3 ? value >> 1 : 0x30; // shift one to the right while value
        // from last iteration is greater than 000011, otherwise go back to 110000

        LATE = (LATE & 0xFF03) | (value << 2); // put value in leds but shift it first
        // because it starts on RE2

        switches = PORTB & 0x0005; // get value from DS3 and DS1 (only)
        delay(1000/(switches == 5 ? 3.5 : 10.5)); // ms = 1/freq * 1000
    }

    return 0;
}

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < K * ms);
}
#include <detpic32.h>

#define K PBCLK/1000

void delay(unsigned int );
void send2displays(unsigned char );

int main(void) {
    TRISB = TRISB & 0x80FF; // RB14-8 as outputs
    TRISD = TRISD & 0x9F; // RD6-5 as outputs
    while(1) {
        send2displays(0x15);
        delay(200);
    }
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
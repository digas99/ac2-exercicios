#include <detpic32.h>

#define K PBCLK/1000

void send2displays(unsigned char );
void delay(unsigned int );
unsigned char toBcd(unsigned char );

int main(void) {
    TRISB = TRISB & (0x80FF); // set displays as outputs
    TRISDbits.TRISD5 = 0; // set RD6-5 as outputs
    TRISDbits.TRISD6 = 0;
    TRISE = TRISE & (0xFF00); // set LEDs as outputs

    int i, counter = 0;
    while(1) {
        i = 0;
        do {
            send2displays(toBcd(counter));
            // put value in LEDs
            LATE = (LATE & (0xFF00)) | counter;

            // display refresh frquency
            // ms = 1/100Hz = 0,01s = 10ms
            delay(10);

            // counter frequency
            // ms = 1/2Hz = 0,5s = 500ms
            // 500ms/10ms = 50
        } while(++i < 50);
        
        counter = counter < 60 ? counter+1 : 0;
    }
    return 0;
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
#include <detpic32.h>

void delay(unsigned int );
void setPWM(unsigned int );

int main(void) {
    TRISB = TRISB | 0x09; // set RB3 & RB0 as inputs

    // Kpresc = PBCLK/(65535*280) = 1,9 -> 2
    PR2 = PBCLK/280/2 - 1;
    T2CONbits.TCKPS = 1;	// 1:2 Timer B
	TMR2 = 0;
	T2CONbits.TON = 1;
    OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 0;
    OC1CONbits.ON = 1;

    while(1) {
        if (PORTBbits.RB3 == 1 && PORTBbits.RB0 == 0)
            setPWM(25);
        if (PORTBbits.RB3 == 0 && PORTBbits.RB0 == 1)
            setPWM(70);
        
        putChar('.');

        delay(250);
    }
    
    return 0;
}

void delay(unsigned int micros) {
    resetCoreTimer();
    // (it takes 1 second for the core timer to go from 0 to 20 000 000 (PBCLK), so
    // it takes 0.001 seconds (1ms) for it to go from 0 to 20 000 (PBCLK/1000), and
    // it takes 0.000001 seconds (1 microsecond) for it to go from 0 to 20 (PBCLK/1000/1000))
    while(readCoreTimer() < (PBCLK/1000/1000) * micros);
}

void setPWM(unsigned int dutyCycle) {
    if (dutyCycle >= 0 && dutyCycle <= 100)
        OC1RS = ((PR2 + 1) * dutyCycle) / 100;
}
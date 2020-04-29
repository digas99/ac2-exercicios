#include <detpic32.h>
#include "functions.h"

void setPWM(unsigned int );

int main(void) {

    // config timer
    /* T3 */
    // config timer T3 with freq = 100Hz  
    T3CONbits.TCKPS = 2; // Kprescaler = 3, logo escolher o 4
    PR3 = 49999; // Fout = 20000KHz/4 = 5000KHz
                 // PR3 = (5000*10^3/100)Hz - 1 = 49999
    TMR3 = 0;
    T3CONbits.TON = 1;

    // duty-cycle is 25%
    OC1CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL = 1; // Use timer T3 as the time base for PWM generation
    setPWM(25);
    OC1CONbits.ON = 1; // Enable OC1 module

    int dutyCycles[21] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};
    int i;

    while(1) {
        for (i = 0; i < 21; i++) {
            setPWM(dutyCycles[i]);
            
            delay(250);
        }
    }

    return 0;
}

void setPWM(unsigned int dutyCycle) {
    if (dutyCycle >= 0 && dutyCycle <= 100) {
        OC1RS = ((PR3 + 1)*dutyCycle)/100;
    }
}
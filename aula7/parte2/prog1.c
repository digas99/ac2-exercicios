#include <detpic32.h>

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
    OC1CONbits.OCTSEL = 0; // Use timer T3 as the time base for PWM generation
    OC1RS = ((PR3 + 1)*25)/100; // Ton constant
    OC1CONbits.ON = 1; // Enable OC1 module
    
    return 0;
}
#include <detpic32.h>
#include "adconfig.h"

int main(void) {
    // config A/D
    adconfig(4, 4, 1, 16);

    // config interrupt system
    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
    IFS1bits.AD1IF = 0; // clear A/D interrupt flag
    IEC1bits.AD1IE = 1; // enable A/D interrupts

    EnableInterrupts(); // global interrupt enable

    // Start A/D convertion
    while(1) {
        /* ALL ACTIVITY IS DONE BY THE ISR */
    }

    return 0;
}

// Interrupt Handler
void _int_(27) isr_adc(void) { // 27 is the vector number of A/D ()
                                  // "PIC32 family datasheet" (pages 74-76)
    // ISR actions
    printInt(ADC1BUF0, 16 | 3 << 16); // print value in hexadecimal,
                                      // 3 digits format
    AD1CON1bits.ASAM = 1; // start convertion
    IFS1bits.AD1IF = 0; // manual reset of AD1IF flag
}
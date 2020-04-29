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

    TRISEbits.TRISE0 = 0; // config RE0 as output

    AD1CON1bits.ASAM = 1; // start convertion
    while(1) {
        /* ALL ACTIVITY IS DONE BY THE ISR */
    }

    return 0;
}

volatile int adc_value;
volatile int rdTime;

// Interrupt Handler
void _int_(27) isr_adc(void) { // 27 is the vector number of A/D ()
                                  // "PIC32 family datasheet" (pages 74-76)
    // ISR actions
    rdTime = readCoreTimer();
    printStr("\nTempo de latência = ");
    printInt10(rdTime*50-3600); // valor do tempo de conversão é 3600ns
                                // (calculado anteriormente)
    printStr("ns");

    LATEbits.LATE0 = 0; // deactivate RE0

    adc_value = ADC1BUF0;

    LATEbits.LATE0 = 1; // activate RE0

    resetCoreTimer();

    AD1CON1bits.ASAM = 1; // start convertion
    IFS1bits.AD1IF = 0; // manual reset of AD1IF flag
}
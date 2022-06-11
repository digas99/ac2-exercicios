#include <detpic32.h>

int main(void) {
    int samples = 4;
    
    // setup ADC
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = samples-1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    // setup interrupt system
    IPC6bits.AD1IP = 2;
    IFS1bits.AD1IF = 0;
    IEC1bits.AD1IE = 1;
    EnableInterrupts();

    // start A/D convertion
    AD1CON1bits.ASAM = 1;

    while(1);

    return 0;
}

// Vector Number for AD1 – ADC1 Convert Done: 27
// PIC32MX7xx_Family_Datasheet - page 75 - line 1 in the table
void _int_(27) isr_adc(void) {
    printInt(ADC1BUF0, 16 | 4 << 16);

    // start A/D convertion
    AD1CON1bits.ASAM = 1;
    
    IFS1bits.AD1IF = 0;
}
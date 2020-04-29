#include <detpic32.h>
#include "adconfig.h"
#include "functions.h"

volatile unsigned char voltage = 0;

int main(void) {
    // config A/D
    adconfig(4, 4, 4, 16);

    // config A/D interrupt system
    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
    IFS1bits.AD1IF = 0; // clear A/D interrupt flag
    IEC1bits.AD1IE = 1; // enable A/D interrupts

    // display 7segs
    LATDbits.LATD6 = 1; // display high active 
    LATDbits.LATD5 = 0; // display low inactive
    TRISB = TRISB & 0x00FF; // configurar RB8-RB15 como saídas
    TRISDbits.TRISD5 = 0; // RD5-RD6 output
    TRISDbits.TRISD6 = 0; // 

    // config timers
    /* T1 */
    // config timer T1 with freq = 4Hz  
    T1CONbits.TCKPS = 3; // Kprescaler = (FPBCLK/4)Hz / 65536 = (20kk/4) / 65536 = 153
                         // Kprescaler = 76 para freq 4Hz, logo escolher o 256,
                         // que é TCKPS = 3
    PR1 = 19499; // Fout = 20000KHz/256 = 78KHz
                 // PR1 = (78*10^3/4)Hz - 1 = 19499
    TMR1 = 0; // Reset timer T1 count register
    T1CONbits.TON = 1; // Enable timer T1 (must be the last command of the
                       // timer configuration sequence)

    // config timer T1 for interruptions
    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T1 interrupts
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag
    
    /* T3 */
    // config timer T3 with freq = 100Hz  
    T3CONbits.TCKPS = 2; // Kprescaler = 3, logo escolher o 4
    PR3 = 49999; // Fout = 20000KHz/4 = 5000KHz
                 // PR3 = (5000*10^3/100)Hz - 1 = 49999
    TMR3 = 0;
    T3CONbits.TON = 1;

    // config timer T3 for interruptions
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;

    EnableInterrupts();

    while(1);

    return 0;
}

// Interrupt Handlers

void _int_(4) isr_T1(void) {
    AD1CON1bits.ASAM = 1; // start convertion
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag
}

void _int_(12) isr_T3(void) {
    send2displays(toBcd(voltage), 1);
    IFS0bits.T3IF = 0;
}

void _int_(27) isr_adc(void) { // 27 is the vector number of A/D ()
                               // "PIC32 family datasheet" (pages 74-76)
    // ISR actions
    int sum=0;

    int *p = (int *)(&ADC1BUF0);
    for (; p <= (int *)(&ADC1BUFF); p+=4) {
        sum += *p;
    }
    voltage = ((sum/4*33)/1023);
    IFS1bits.AD1IF = 0; // manual reset of AD1IF flag
}
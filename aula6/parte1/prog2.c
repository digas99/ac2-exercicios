#include <detpic32.h>
#include "adconfig.h"
#include "functions.h"

volatile unsigned char voltage = 0;

int main(void) {
    unsigned int cnt = 0;

    // config A/D
    adconfig(4, 4, 4, 16);

    // config interrupt system
    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
    IFS1bits.AD1IF = 0; // clear A/D interrupt flag
    IEC1bits.AD1IE = 1; // enable A/D interrupts

    EnableInterrupts(); // global interrupt enable

    // display 7segs
    LATDbits.LATD6 = 1; // display high active 
    LATDbits.LATD5 = 0; // display low inactive
    TRISB = TRISB & 0x00FF; // configurar RB8-RB15 como saídas
    TRISDbits.TRISD5 = 0; // RD5-RD6 output
    TRISDbits.TRISD6 = 0; // 

    // Start A/D convertion
    while(1) {
        if (cnt % 25 == 0) { // 250ms (4 samples/sec)
            AD1CON1bits.ASAM = 1; // start convertion
        }
        send2displays(toBcd(voltage), 1);
        cnt++;
        delay(10);
    }

    return 0;
}

// Interrupt Handler
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
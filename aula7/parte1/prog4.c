#include <detpic32.h>

int main(void) {

    /* T1 */
    // config timer T1 with freq = 2Hz  
    T1CONbits.TCKPS = 3; // Kprescaler = (FPBCLK/2)Hz / 65536 = (20kk/2) / 65536 = 153
                         // Kprescaler = 153 para freq 2Hz, logo escolher o 256,
                         // que é TCKPS = 3
    PR1 = 38999; // Fout = 20000KHz/256 = 78KHz
                 // PR1 = (78*10^3/2)Hz - 1 = 38999
    TMR1 = 0; // Reset timer T1 count register
    T1CONbits.TON = 1; // Enable timer T1 (must be the last command of the
                       // timer configuration sequence)

    // config timer T1 for interruptions
    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T1 interrupts
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag
    
    /* T3 */
    // config timer T3 with freq = 20Hz  
    T3CONbits.TCKPS = 4; // Kprescaler = 15, logo escolher o 16
    PR3 = 62449; // Fout = 20000KHz/16 = 1250KHz
                 // PR3 = (1250*10^3/20)Hz - 1 = 62499
    TMR3 = 0;
    T3CONbits.TON = 1;

    // config timer T3 for interruptions
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;

    EnableInterrupts();

    while(1);
}

void _int_(4) isr_T1(void) {
    putChar('1');
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag
}

void _int_(12) isr_T3(void) {
    putChar('3');
    IFS0bits.T3IF = 0;
}
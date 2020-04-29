#include <detpic32.h>

int main(void) {
    
    // config timer T3 with freq = 2Hz  
    T3CONbits.TCKPS = 7; // Kprescaler = (FPBCLK/2)Hz / 65536 = (20kk/2) / 65536 = 153
                         // Kprescaler = 153 para freq 2Hz, logo escolher o 256,
                         // que é TCKPS = 7
    PR3 = 38999; // Fout = 20000KHz/256 = 78KHz
                 // PR3 = (78*10^3/2)Hz - 1 = 38999
    TMR3 = 0; // Reset timer T3 count register
    T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the
                       // timer configuration sequence)

    // config timer T3 for interruptions
    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag

    EnableInterrupts();

    while(1);
}

void _int_(12) isr_T3(void) {
    putChar('.');
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag
}
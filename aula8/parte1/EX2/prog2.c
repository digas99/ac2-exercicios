#include <detpic32.h>

int main(void) {
    // Fout = 2Hz   Fpbclk = 20MHz  PR3(max value) = 2^16-1 = 65535
    // Kpresc = fpbclk/(PR3+1)*Fout = 20000000/(65536*2) = 153
    // Kpresc = 153 -> 256
    // PR3 = (Fpbclk/Kpresc)/Fout - 1
    PR3 = (PBCLK/256)/2 - 1; // PR3 = 39062
    T3CONbits.TCKPS = 7; // 1:256 prescaler
    TMR3 = 0; // reset timer T3 count register
    T3CONbits.TON = 1; // enable timer T3
    
    IPC3bits.T3IP = 2;  // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts
    IFS0bits.T3IF = 0; // Reset timer T3 Interrupt flag
    EnableInterrupts();

    while(1);

    return 0;
}

void _int_(12) isr_T3(void) {
    putChar('.');

    IFS0bits.T3IF = 0; // Reset timer T3 Interrupt flag
}
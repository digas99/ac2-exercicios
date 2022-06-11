#include <detpic32.h>

int main(void) {
    // Timer 1:
    // Fout = 5Hz   Fpbclk = 20MHz  PR1(max value) = 2^16-1 = 65535
    // Kpresc = fpbclk/(PR1+1)*Fout = 20000000/(65536*5) = 61
    // Kpresc = 61 -> 64
    // PR1 = (Fpbclk/Kpresc)/Fout - 1
    PR1 = (PBCLK/64)/5 - 1; // PR1 = 62499
    T1CONbits.TCKPS = 2; // 1:64 prescaler
    TMR1 = 0; // reset timer T1 count register
    T1CONbits.TON = 1; // enable timer T1
    
    IPC1bits.T1IP = 2;  // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T1 interrupts
    IFS0bits.T1IF = 0; // Reset timer T1 Interrupt flag

    // Timer 3:
    // Fout = 25Hz   Fpbclk = 20MHz  PR3(max value) = 2^16-1 = 65535
    // Kpresc = fpbclk/(PR3+1)*Fout = 20000000/(65536*25) = 12
    // Kpresc = 12 -> 16
    // PR3 = (Fpbclk/Kpresc)/Fout - 1
    PR3 = (PBCLK/16)/25 - 1; // PR3 = 49999
    T3CONbits.TCKPS = 4; // 1:16 prescaler
    TMR3 = 0; // reset timer T3 count register
    T3CONbits.TON = 1; // enable timer T3
    
    IPC3bits.T3IP = 2;  // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts
    IFS0bits.T3IF = 0; // Reset timer T3 Interrupt flag

    EnableInterrupts();

    while(1);

    return 0;
}

void _int_(4) isr_T1(void) {
    putChar('1');
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void) {
    putChar('3');
    IFS0bits.T3IF = 0;
}
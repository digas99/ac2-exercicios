#include <detpic32.h>

int main(void) {
    // config Timer 2
    // For 3 seconds (0,333Hz), configure timer for 500ms (2Hz) and
    // run program at every 6th interruption (500ms*6 = 3s)
    // (can't configure timer directly for Fout=0,333Hz because of the prescaler)

    // Fout = 2Hz   Fpbclk = 20MHz  PR3(max value) = 2^16-1 = 65535
    // Kpresc = fpbclk/(PR3+1)*Fout = 20000000/(65536*2) = 153
    // Kpresc = 153 -> 256
    // PR3 = (Fpbclk/Kpresc)/Fout - 1
    PR2 = (PBCLK/256)/2 - 1; // PR2 = 39062
    T2CONbits.TCKPS = 7; // 1:256 prescaler
    TMR2 = 0; // reset timer T2 count register
    T2CONbits.TON = 1; // enable timer 2

    // Timer 2 interrupts
    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 Interrupt flag

    TRISDbits.TRISD8 = 1; // set RD8 as input
    TRISEbits.TRISE0 = 0; // set RE0 as output

    // INT1 interrupts
    IPC1bits.INT1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.INT1IE = 1; // Enable INT1 interrupts
    IFS0bits.INT1IF = 0; // Reset INT1 Interrupt flag

    // INT1EP: External Interrupt 1 Edge Polarity Control bit
    // 1 = Rising edge
    // 0 = Falling edge
    INTCONbits.INT1EP = 0;

    EnableInterrupts();

    while(1);

    return 0;
}

volatile int cnt = 0;

void _int_(8) isr_T2(void) {
    if (cnt == 0)
        LATEbits.LATE0 = 0;
    
    // cnt is 0 at every 6 interruptions
    cnt = (cnt + 1) % 6;

    IFS0bits.T2IF = 0;
}

void _int_(7) isr_INT1(void) {
    LATEbits.LATE0 = 1;
    IFS0bits.INT1IF = 0;
}
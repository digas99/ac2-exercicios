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

    while(1) {
        while(IFS0bits.T3IF == 0);
        IFS0bits.T3IF = 0;

        putChar('.');
    }

    return 0;
}
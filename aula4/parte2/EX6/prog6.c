#include <detpic32.h>

int main(void) {
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    TRISB = TRISB | 0x0F; // RB3-0 as inputs
    TRISB = TRISB & 0x80FF; // RB14-8 as outputs
    TRISD = TRISD & 0x9F; // RD6-5 as outputs
    LATD = (LATD & 0x9F) | 0x20; // RD5=1 and RD6=1
    while(1) {
        char disp_switch = PORTB & 0x0F;
        LATB = (LATB & 0x80FF) | (display7Scodes[disp_switch] << 8);
    }
    return 0;
}
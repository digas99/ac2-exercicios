#include <detpic32.h>

#define K PBCLK/1000

void send2displays(unsigned char );
void delay(unsigned int );
unsigned char toBcd(unsigned char );
void print_value(unsigned char , unsigned int );

int main(void) {
    TRISB = TRISB & (0x80FF); // set displays as outputs
    TRISDbits.TRISD5 = 0; // set RD6-5 as outputs
    TRISDbits.TRISD6 = 0;
    TRISE = TRISE & (0xFF00); // set LEDs as outputs
    TRISBbits.TRISB0 = 1; // set RB0 as input

    int i, j, k, blink_ms, counter_freq = 2, counter = 0;
    while(1) {
        i = 0;

        do {
            // ms = 1/100Hz = 0,01s = 10ms
            print_value(counter, 10);

            // counter frequency
            // ms = 1/2Hz = 0,5s = 500ms
            // 500ms/10ms = 50
        } while(++i < (1000/counter_freq)/10);

        if (PORTBbits.RB0) {
            counter++;
            counter_freq = 5;
        }
        else {
            counter--;
            counter_freq = 2;
        }

        if (counter == -1 || counter == 61) {
            j = 0;
            
            if (counter == -1) counter = 60;
            else if (counter == 61) counter = 0;

            do {
                TRISDbits.TRISD5 = !TRISDbits.TRISD5;
                TRISDbits.TRISD6 = !TRISDbits.TRISD6;

                blink_ms = PORTBbits.RB0 ? 200 : 500;
                k = 0;
                do {
                    print_value(counter, 10);

                    // ms = blink_ms
                    // blink_ma/10ms
                } while(++k < blink_ms/10);

                // ms = 5s = 5000ms
                // 5000ms/blink_ms
            } while(++j < 5000/blink_ms);
            
            TRISDbits.TRISD5 = 0;
            TRISDbits.TRISD6 = 0;
        }
    }
    return 0;
}

void send2displays(unsigned char value) {
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;
    
    if (!displayFlag) {
        LATDbits.LATD6 = 0;
        LATDbits.LATD5 = 1;
        LATB = (LATB & 0x80FF) | (display7Scodes[(value & 0x0F)] << 8);
    }
    else {
        LATDbits.LATD6 = 1;
        LATDbits.LATD5 = 0;
        LATB = (LATB & 0x80FF) | (display7Scodes[(value >> 4)] << 8);    
    }

    displayFlag = !displayFlag;
}

void delay(unsigned int ms) {
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}

unsigned char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}

void print_value(unsigned char value, unsigned int ms) {
    send2displays(toBcd(value));
    // put value in LEDs
    LATE = (LATE & (0xFF00)) | value;

    // display refresh frquency
    delay(ms);
}
#include <detpic32.h>

#define K PBCLK/1000

void delay(unsigned int );

void main(void) {
	unsigned char segment;
	int i;
	LATD = (LATD & 0xFF9F) | 0x40; // RD6=1, RD5=0
	TRISB = TRISB & 0x80FF; // RB14-8 as outputs 
	TRISD = TRISD & 0xFF9F; // RD6-5 as outputs
	while(1) {
		segment = 1;
		for (i = 0; i < 7; i++) {
			LATB = (LATB & 0x80FF) | (segment << 8);
            // 10Hz: 1/10s = 0,1s = 100ms
            // 50Hz: 1/50s = 0,02s = 20ms
            // 100Hz: 1/100s = 0.01s = 10ms
			delay(10);
			segment = segment << 1;
		}
		LATDbits.LATD5 = !LATDbits.LATD5;
		LATDbits.LATD6 = !LATDbits.LATD6;
	}
}

void delay(unsigned int ms) {
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}
#include <detpic32.h>

#define K PBCLK/1000

void delay(unsigned int );

void main(void) {
	TRISCbits.TRISC14 = 0; // define RC14 as exit
	while(1) {
		delay(500);
		LATCbits.LATC14 = !LATCbits.LATC14;
	}
}

void delay(unsigned int ms) {
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}
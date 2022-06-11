#include <detpic32.h>

#define K PBCLK/1000

void delay(unsigned int );

void main(void) {
	TRISE = TRISE & 0xFFF0; // RE3-0 as outputs
	int counter = 0;
	while(1) {
		if (counter%16 == 0) counter = 0; 
		counter++;

		LATE = (LATE & 0xFFF0) | counter; // put value of counter in RE3-0

		// 4Hz -> 1/4Hz = 0,25s = 250ms
		delay(250);
	}
}

void delay(unsigned int ms) {
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}
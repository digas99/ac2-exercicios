#include <detpic32.h>

void main(void) {
	unsigned int codes[] = {1, 2, 4, 8, 16, 32, 64};
	unsigned int i;
	TRISB = TRISB & 0x80FF; // RB14-8 as outputs
	TRISD = TRISD & 0xFF9F; // RD6-5 as outputs

	LATD = (LATD & 0xFF9F) | 0x20; // RD5=1, RD6=0
	while(1) {
		i = getChar() - 'a';
		LATB = (LATB & 0x80FF) | (codes[i] << 8);
	} 
}
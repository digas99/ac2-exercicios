#include <detpic32.h>
#include "functions.h"

int main(void) {
	TRISE = TRISE & 0xFFF0;
	int ms, counter = 0;

	while(1) {
		LATE = LATE & 0xFFF0;
		LATE = LATE | counter;
		ms = dip_switch_ms();
		delay(ms);
		printInt10(counter);
		putChar('\n');
		printInt(counter, 2);
		putChar('\n');
		counter = ++counter % 16; 	
	}
}

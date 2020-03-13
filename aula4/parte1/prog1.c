#include <detpic32.h>
#include "functions.h"

int main(void) {
	LATEbits.LATE0 = 0;
	LATEbits.LATE1 = 0;
	LATEbits.LATE2 = 0;
	LATEbits.LATE3 = 0;

	TRISE = (TRISE & 0xFFF0); // Os 4 bits menos significativos ficam a 0
									  // Ou seja, ficam configurados como portos de saída
	int ms;
	while(1) {
		ms = dip_switch_ms();
		LATEbits.LATE0 = 1;
		delay(ms);
		LATEbits.LATE0 = 0;
		LATEbits.LATE1 = 1;
		delay(ms);
		LATEbits.LATE1 = 0;
		LATEbits.LATE2 = 1;
		delay(ms);
		LATEbits.LATE2 = 0;
		LATEbits.LATE3 = 1;
		delay(ms);
		LATEbits.LATE3 = 0;
	}
	return 0;
}

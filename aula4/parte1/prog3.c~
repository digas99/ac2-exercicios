#include <detpic32.h>


int main(void) {
	char c;
	TRISB = TRISB & 0x00FF; // Os portos RB8 a RB15 ficam configurados
  							  // como portos de saída
	TRISDbits.TRISD5 = 0; // RD5 configurado como porto de saída
	TRISDbits.TRISD6 = 0; // RD6 configurado como porto de saída

	LATDbits.LATD5 = 0; // selecionar o 'display low'
	LATDbits.LATD6 = 1;

	while(1) {
		c = getChar();
		LATB = LATB & 0x00FF;
		switch(c) {
			case 'a': case 'A':
				LATBbits.LATB8 = 1;
				break;
			case 'b': case 'B':
				LATBbits.LATB9 = 1;
				break;
			case 'c': case 'C':
				LATBbits.LATB10 = 1;
				break;
			case 'd': case 'D':
				LATBbits.LATB11 = 1;
				break;
			case 'e': case 'E':
				LATBbits.LATB12 = 1;
				break;
			case 'f': case 'F':
				LATBbits.LATB13 = 1;
				break;
			case 'g': case 'G':
				LATBbits.LATB14 = 1;
				break;
			case '.':
				LATBbits.LATB15 = 1;
				break;
		}
	}
}

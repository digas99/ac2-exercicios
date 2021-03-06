#include <detpic32.h>
#include "functions.h"

int main(void) {
	unsigned char segment;
	int i, ms;
	LATDbits.LATD6 = 1; // display high active
	LATDbits.LATD5 = 0; //
	TRISB = TRISB & 0x00FF; // RB8-RB15 configurados como outputs
	TRISDbits.TRISD5 = 0; // RD5-RD6 configurados como outputs
	TRISDbits.TRISD6 = 0; //
		
	while(1) {
		LATDbits.LATD6 = !LATDbits.LATD6; //
		LATDbits.LATD5 = !LATDbits.LATD5; // toggle display selection
		segment = 1;
		for (i = 0; i < 7; i++) {
			LATB = ((LATB & 0x0000) | segment) << 8; // resetar o valor do LATB; dar o valor do segment;
																  // dar um shift para a esquerda de 8 casas
																  // começa em RB8
			ms = dip_switch_ms();
			delay(ms);
			segment = segment << 1;
		}
	}
	return 0;
}

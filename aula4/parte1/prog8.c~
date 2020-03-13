#include <detpic32.h>
#include "functions.h"

int main(void) {
	static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	TRISB = TRISB | 0x000F; // configurar RB0-RB3 como entradas
	TRISB = TRISB & 0x80FF; // configurar RB8-RB14 como saídas
	TRISDbits.TRISD5 = 0; // RD5-RD6 output
	TRISDbits.TRISD6 = 0; // 
	LATDbits.LATD5 = 1; // select display low
	LATDbits.LATD6 = 0; //
	int dip_switch_val;

	while(1) {
   	dip_switch_val = PORTB & 0x000F; // isolar os valores desejados do dip_switch
		LATB = LATB & 0x80FF; // resetar os valores do RB8-RB14 (LATB & 1000 0000 1111 1111)
		LATB = LATB | (display7Scodes[dip_switch_val] << 8); // atribuir o valor do display correspondente
																			  // ao valor recebido pelos switches
																			  // como vamos alterar os bits menos
																			  // significativos, são feits 8 shifts left
	}
}

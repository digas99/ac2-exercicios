#include <detpic32.h>
#include "functions.h"

void send2displays(unsigned char );
int dip_switch_ms_modified();

int main(void) {
	TRISB = TRISB | 0x000F; // configurar RB0-RB3 como entradas
   TRISB = TRISB & 0x80FF; // configurar RB8-RB14 como saídas
   TRISDbits.TRISD5 = 0; // RD5-RD6 output
   TRISDbits.TRISD6 = 0; // 
	int dip_switch_val;

   while(1) {
      dip_switch_val = PORTB & 0x000F; // isolar os valores desejados do dip_switch
		send2displays(0xF3);
   }
}

void send2displays(unsigned char value) {
	static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	unsigned char disp_low = value & 0x0F;
	unsigned char disp_high = value >> 4;
	int ms = dip_switch_ms_modified();
		
	printStr("Display Low: ");
	printInt(disp_low, 16);
	putChar('\n');
	LATDbits.LATD5 = 1; // select display low
   LATDbits.LATD6 = 0; //
	LATB = LATB & 0x80FF; // resetar os valores do RB8-RB14 (LATB & 1000 0000 1111 1111)
	LATB = LATB | (display7Scodes[disp_low] << 8);
		
	delay(ms);

	printStr("Display High: ");
	printInt(disp_high, 16);
	putChar('\n');
	LATDbits.LATD5 = 0; // select display high
   LATDbits.LATD6 = 1; //
	LATB = LATB & 0x80FF; // resetar os valores do RB8-RB14 (LATB & 1000 0000 1111 1111)
	LATB = LATB | (display7Scodes[disp_high] << 8);

	delay(ms);
}

int dip_switch_ms_modified() {
   int ms;
   ms = PORTB & 0x000F;
   ms = 1000/((ms+1)*6); // 1000/valor dos switches+100
   printInt10(ms);
   printStr(" ms");
   putChar('\n');
   return ms;
}

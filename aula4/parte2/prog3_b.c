#include <detpic32.h>

void send2displays(unsigned char );
void delay(int );

int main(void) {
   TRISB = TRISB | 0x000F; // configurar RB0-RB3 como entradas
   TRISB = TRISB & 0x80FF; // configurar RB8-RB14 como saídas
   TRISDbits.TRISD5 = 0; // RD5-RD6 output
   TRISDbits.TRISD6 = 0; // 
   int i, counter = 0;

   while(1) {
		i = 0;  
		do {
      	send2displays(counter);
			delay(50);
		} while (++i < 4);
		counter++;
   }

	return 0;
}

void send2displays(unsigned char value) {
   static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
   unsigned char disp_low = value & 0x0F;
   unsigned char disp_high = value >> 4;
	static char displayFlag = 0;

	if (!displayFlag) {
		printStr("Display Low: ");
		printInt(disp_low, 16);
		putChar('\n');
		LATDbits.LATD5 = 1; // select display low
		LATDbits.LATD6 = 0; //
		LATB = LATB & 0x80FF; // resetar os valores do RB8-RB14 (LATB & 1000 0000 1111 1111)
		LATB = LATB | (display7Scodes[disp_low] << 8);
	}
	else {
		printStr("Display High: ");
		printInt(disp_high, 16);
		putChar('\n');
		LATDbits.LATD5 = 0; // select display high
		LATDbits.LATD6 = 1; //
		LATB = LATB & 0x80FF; // resetar os valores do RB8-RB14 (LATB & 1000 0000 1111 1111)
		LATB = LATB | (display7Scodes[disp_high] << 8);
	}

	displayFlag = !displayFlag;
}

void delay(int ms) {
   for (; ms > 0; ms--) {
      resetCoreTimer();
      while (readCoreTimer() < PBCLK/1000); 
   }
}

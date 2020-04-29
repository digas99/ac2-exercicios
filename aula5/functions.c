#include <detpic32.h>

void send2displays(unsigned char value, int comma) {
   static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
   unsigned char disp_low = value & 0x0F;
   unsigned char disp_high = value >> 4;
   static char displayFlag = 0;

   if (!displayFlag) {
      LATDbits.LATD5 = 1; // select display low
      LATDbits.LATD6 = 0; //
      LATB = LATB & 0x00FF; // resetar os valores do RB8-RB15 (LATB & 0000 0000 1111 1111)
      LATB = LATB | (display7Scodes[disp_low] << 8);
   }
   else {
      LATDbits.LATD5 = 0; // select display high
      LATDbits.LATD6 = 1; //
      LATB = LATB & 0x00FF; // resetar os valores do RB8-RB15 (LATB & 0000 0000 1111 1111)
      LATB = LATB | (display7Scodes[disp_high] << 8);
      if (comma)
			LATB = LATB | 0x8000;
   }
	
	displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value) {
	return ((value / 10) << 4) + (value % 10);
}

void delay(int ms) {
   for (; ms > 0; ms--) {
      resetCoreTimer();
      while (readCoreTimer() < PBCLK/1000);
   }
}
#include <detpic32.h>

void send2displays(unsigned char ,int );
void delay(int );
unsigned char toBcd(unsigned char );
void zeroFlicker();

int main(void) {
   TRISB = TRISB | 0x000F; // configurar RB0-RB3 como entradas
   TRISB = TRISB & 0x00FF; // configurar RB8-RB15 como saídas
   TRISDbits.TRISD5 = 0; // RD5-RD6 output
   TRISDbits.TRISD6 = 0; // 
   int i, par, counter = 0;

   while(1) {
      i = 0;
		if (counter > 99) {
			counter = 0;
			zeroFlicker();
		}

		if (counter % 2 == 0)
			par = 1;
		else
			par = 0;
      
		do {
				send2displays(toBcd(counter), par);
         delay(10); // refresh frequency
      } while (++i < 100); // increment frequency
      counter++;
   }

   return 0;
}

void send2displays(unsigned char value, int par) {
   static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
   unsigned char disp_low = value & 0x0F;
   unsigned char disp_high = value >> 4;
   static char displayFlag = 0;

   if (!displayFlag) {
      //printStr("Display Low: ");
      //printInt(disp_low, 16);
      //putChar('\n');
      LATDbits.LATD5 = 1; // select display low
      LATDbits.LATD6 = 0; //
      LATB = LATB & 0x00FF; // resetar os valores do RB8-RB15 (LATB & 0000 0000 1111 1111)
      LATB = LATB | (display7Scodes[disp_low] << 8);
		if (par)
			LATB = LATB | 0x8000;
   }
   else {
      //printStr("Display High: ");
      //printInt(disp_high, 16);
      //putChar('\n');
      LATDbits.LATD5 = 0; // select display high
      LATDbits.LATD6 = 1; //
      LATB = LATB & 0x00FF; // resetar os valores do RB8-RB15 (LATB & 0000 0000 1111 1111)
      LATB = LATB | (display7Scodes[disp_high] << 8);
		if (!par)
			LATB = LATB | 0x8000;
   }
	
	displayFlag = !displayFlag;
}

void delay(int ms) {
   for (; ms > 0; ms--) {
      resetCoreTimer();
      while (readCoreTimer() < PBCLK/1000);
   }
}
           
unsigned char toBcd(unsigned char value) {
	return ((value / 10) << 4) + (value % 10);
}

void zeroFlicker() {
	int i;
	static char flag = 0;
	for (i = 0; i < 10; i++) {
		if (!flag) {
			LATDbits.LATD5 = 1; // select display high
			LATDbits.LATD6 = 1; //
			LATB = LATB & 0x00FF;
			LATB = LATB | (0x3F << 8);
		}
		else {
			LATDbits.LATD5 = 0; // select display high
			LATDbits.LATD6 = 0; //
			LATB = LATB & 0x00FF;
			LATB = LATB | (0x3F << 8);
		}
		flag = !flag;

		delay(500);
	}
}

#include <detpic32.h>

void delay(int );
int dip_switches_ms();


void delay(int ms) {
   for (; ms > 0; ms--) {
      resetCoreTimer();
      while (readCoreTimer() < PBCLK);
   }
}

int dip_switch_ms() {
   int ms;
   ms = PORTB & 0x000F;
   ms = 1000/(ms+1); // 1000/valor dos switches+1
   printInt10(ms);
   printStr(" ms");
   putChar('\n');
   return ms;
}

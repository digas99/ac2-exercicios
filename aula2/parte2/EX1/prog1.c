#include <detpic32.h>

#define K 20000

void delay(int );

int main(void) {
	int cnt1 = 0, cnt5 = 0, cnt10 = 0;
	// 10Hz = 100ms, 5Hz = 200ms, 1Hz = 1000ms
	int frq1 = 10, frq5 = 5, frq10 = 1;
	while(1) {
		cnt1++;
		if (cnt1%(frq1/frq5) == 0) cnt5++;
		if (cnt1%(frq1/frq10) == 0) cnt10++;
		
		putChar('\r');
		printInt(cnt1, 10 | 5 << 16);
		putChar(' ');
		printInt(cnt5, 10 | 5 << 16);
		putChar(' ');
		printInt(cnt10, 10 | 5 << 16);

		// 1/frq1s == 1/frq1*1000ms == 1000/frq1ms
		delay(1000/frq1);
	}
	return 0;
}

void delay(int ms) {
	resetCoreTimer();
	while(readCoreTimer() < K * ms);
}
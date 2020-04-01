#include <detpic32.h>
#include "adconfig.h"
#include "functions.h"

int main(void) {

    adconfig(4, 4, 4, 16);

    while(1) {
        int voltage, sum=0;

        // start convertion
        AD1CON1bits.ASAM = 1;

        // esperar até que a conversão termine (o bit AD1IF ficará a 1)
        while(IFS1bits.AD1IF == 0);

        int *p = (int *)(&ADC1BUF0);
        for (; p <= (int *)(&ADC1BUFF); p+=4) {
            printInt(*p, 16 | 4 << 16);
            putChar(' ');

            sum += *p;
        }
        printStr("\nVoltage: ");
        voltage = ((sum/4*33)/1023);
        printInt(voltage, 10);
        putChar('\n');
        delay(500);

        IFS1bits.AD1IF = 0; // reset manual do valor do bit AD1IF
    }

    return 0;
}

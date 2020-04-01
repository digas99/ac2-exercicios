#include <detpic32.h>
#include "adconfig.h"

int main(void) {

    adconfig(4, 4, 1, 16);

    while(1) {
        int i;

        // start convertion
        AD1CON1bits.ASAM = 1;

        // esperar até que a conversão termine (o bit AD1IF ficará a 1)
        while(IFS1bits.AD1IF == 0);

        int *p = (int *)(&ADC1BUF0);
        for (i = 0; i < 16; i++) {
            printInt(p[i*4], 16 | 4 << 16);
            putChar(' ');
        }
        putChar('\n');

        IFS1bits.AD1IF = 0; // reset manual do valor do bit AD1IF
    }

    return 0;
}

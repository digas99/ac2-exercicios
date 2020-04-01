#include <detpic32.h>
#include "adconfig.h"
#include "functions.h"

int main(void) {

    adconfig(4, 4, 4, 16);
    
    // display 7segs
    LATDbits.LATD6 = 1; // display high active 
    LATDbits.LATD5 = 0; // display low inactive

    TRISB = TRISB | 0x000F; // configurar RB0-RB3 como entradas
    TRISB = TRISB & 0x00FF; // configurar RB8-RB15 como saídas
    TRISDbits.TRISD5 = 0; // RD5-RD6 output
    TRISDbits.TRISD6 = 0; // 
    
    int i = 0;

    while(1) {
        int voltage, sum=0;

         delay(10); // refresh frequency

        if (++i == 25) {

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

            i = 0;
        }
        send2displays(toBcd(voltage), 1);

        IFS1bits.AD1IF = 0; // reset manual do valor do bit AD1IF
    }

    return 0;
}

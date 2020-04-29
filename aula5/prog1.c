#include <detpic32.h>
#include "adconfig.h"
#include "functions.h"

int main(void) {
    // // configuração do ADC
    // TRISBbits.TRISB4 = 1; // configurado o porto como entrada analógica
    // AD1PCFGbits.PCFG4 = 0;

    // AD1CHSbits.CH0SA = 4; // seleção do AN4 como o input para o ADC

    // AD1CON1bits.SSRC = 7; // trigger do processo de ínicio de conversão passa
    //                       // para "auto convnert"
    // AD1CON1bits.CLRASAM = 1; // processo de conversão ocorre apenas quando é dada
    //                          // ordem de início
 
    // AD1CON2bits.SMPI = 1-1; // número de conversões sucessivas

    // AD1CON3bits.SAMC = 16; // duração do tempo de amostragem

    // AD1CON1bits.ON = 1; // enable ADC

    adconfig(4, 4, 1, 16);

    int rdTime;
    while(1) {

        resetCoreTimer();

        // start convertion
        AD1CON1bits.ASAM = 1;

        // esperar até que a conversão termine (o bit AD1IF ficará a 1)
        while(IFS1bits.AD1IF == 0);

        rdTime = readCoreTimer();
        printStr("\nTempo de conversão = ");
        printInt10(rdTime*50);
        printStr("ns - ");
        printInt(ADC1BUF0, 16 | 3 << 16); // print do valor em hexadecimal,
                                          // formatado com 3 digitos

        IFS1bits.AD1IF = 0; // reset manual do valor do bit AD1IF

        delay(500);
    }

    return 0;
}

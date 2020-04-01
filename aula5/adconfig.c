#include <detpic32.h>

void adconfig(int port, int an_val, int cons_convs, int time) {
    // configurado o porto como entrada analógica
    switch(port) {
        case 0:
            TRISBbits.TRISB0 = 1;
            AD1PCFGbits.PCFG0 = 0;
            break;
        case 1:
            TRISBbits.TRISB1 = 1;
            AD1PCFGbits.PCFG1 = 0;
            break;
        case 2:
            TRISBbits.TRISB2 = 1;
            AD1PCFGbits.PCFG2 = 0;
            break;
        case 3:
            TRISBbits.TRISB3 = 1;
            AD1PCFGbits.PCFG3 = 0;
            break;
        case 4:
            TRISBbits.TRISB4 = 1;
            AD1PCFGbits.PCFG4 = 0;
            break;
        case 5:
            TRISBbits.TRISB5 = 1;
            AD1PCFGbits.PCFG5 = 0;
            break;
        case 6:
            TRISBbits.TRISB6 = 1;
            AD1PCFGbits.PCFG6 = 0;
            break;
        case 7:
            TRISBbits.TRISB7 = 1;
            AD1PCFGbits.PCFG7 = 0;
            break;
        case 8:
            TRISBbits.TRISB8 = 1;
            AD1PCFGbits.PCFG8 = 0;
            break;
        case 9:
            TRISBbits.TRISB9 = 1;
            AD1PCFGbits.PCFG9 = 0;
            break;
        case 10:
            TRISBbits.TRISB10 = 1;
            AD1PCFGbits.PCFG10 = 0;
            break;
        case 11:
            TRISBbits.TRISB11 = 1;
            AD1PCFGbits.PCFG11 = 0;
            break;
        case 12:
            TRISBbits.TRISB12 = 1;
            AD1PCFGbits.PCFG12 = 0;
            break;
        case 13:
            TRISBbits.TRISB13 = 1;
            AD1PCFGbits.PCFG13 = 0;
            break;
        case 14:
            TRISBbits.TRISB14 = 1;
            AD1PCFGbits.PCFG14 = 0;
            break;
        case 15:
            TRISBbits.TRISB15 = 1;
            AD1PCFGbits.PCFG15 = 0;
            break;
        default:
            return;
    }

    AD1CHSbits.CH0SA = an_val; // canal de entrada

    AD1CON1bits.SSRC = 7; // trigger do processo de ínicio de conversão passa
                          // para "auto convnert"
    AD1CON1bits.CLRASAM = 1; // processo de conversão ocorre apenas quando é dada
                             // ordem de início
 
    AD1CON2bits.SMPI = cons_convs-1; // número de conversões sucessivas

    AD1CON3bits.SAMC = time; // duração do tempo de amostragem

    AD1CON1bits.ON = 1; // enable ADC
}

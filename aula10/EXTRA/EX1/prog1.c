#include <detpic32.h>

// PIC32MX7xx_Family_Datasheet -> pages 207-10
// UxMODE: UARTx MODE REGISTER
//     ON: UARTx Enable bit
//         1 = UARTx is enabled. UARTx pins are controlled by UARTx as defined by UEN<1:0> and UTXEN control bits.
//         0 = UARTx is disabled. All UARTx pins are controlled by corresponding bits in the PORTx, TRISx and LATx registers; UARTx power consumption is minimal.
//     BRGH: High Baud Rate Enable bit
//         1 = High-Speed mode – 4x baud clock enabled
//         0 = Standard Speed mode – 16x baud clock enabled
//     PDSEL<1:0>: Parity and Data Selection bits
//         11 = 9-bit data, no parity
//         10 = 8-bit data, odd parity
//         01 = 8-bit data, even parity
//         00 = 8-bit data, no parity
//     STSEL: Stop Selection bit
//         1 = 2 Stop bits
//         0 = 1 Stop bit
// UxSTA: UARTx STATUS AND CONTROL REGISTER
//     URXEN: Receiver Enable bit
//         1 = UARTx receiver is enabled. UxRX pin is controlled by UARTx (if ON = 1)
//         0 = UARTx receiver is disabled. UxRX pin is ignored by the UARTx module. UxRX pin is controlled by port.
//     UTXEN: Transmit Enable bit
//         1 = UARTx transmitter is enabled. UxTX pin is controlled by UARTx (if ON = 1)
//         0 = UARTx transmitter is disabled. Any pending transmission is aborted and buffer is reset. UxTX pin is controlled by port.

void configUart2(unsigned int baud, char parity, unsigned int stopbits) {
    baud = baud == 600 || baud == 115200 ? baud : 115200;
    U2BRG = ((PBCLK+8*baud) / (16*baud)) - 1;
    U2MODEbits.BRGH = 0;
    
    switch(parity) {
        case 'E':
            U2MODEbits.PDSEL = 0b01;
            break;
        case 'O':
            U2MODEbits.PDSEL = 0b10;
            break;
        default:
            U2MODEbits.PDSEL = 0b00;
    }
    U2MODEbits.STSEL = stopbits == 1 || stopbits == 2 ? stopbits : 1;

    U2STAbits.URXEN = 1;    
    U2STAbits.UTXEN = 1;
    U2MODEbits.ON = 1;
}
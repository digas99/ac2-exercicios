#include <detpic32.h>
#include "adconfig.h"
#include "functions.h"

volatile int voltMin = 33, voltMax = 0;
volatile unsigned char voltage = 0;
void setPWM(unsigned int );
void UART1config(int , char , int );
void putc(char );
void puts(char *);

int main(void) {
    // config A/D
    adconfig(4, 4, 4, 16);

    // config A/D interrupt system
    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
    IFS1bits.AD1IF = 0; // clear A/D interrupt flag
    IEC1bits.AD1IE = 1; // enable A/D interrupts

    // display 7segs
    LATDbits.LATD6 = 1; // display high active 
    LATDbits.LATD5 = 0; // display low inactive
    TRISB = TRISB & 0x00FF; // configurar RB8-RB15 como saídas
    TRISDbits.TRISD5 = 0; // RD5-RD6 output
    TRISDbits.TRISD6 = 0; // 

    // config timers
    /* T1 */
    // config timer T1 with freq = 4Hz  
    T1CONbits.TCKPS = 3; // Kprescaler = (FPBCLK/4)Hz / 65536 = (20kk/4) / 65536 = 153
                         // Kprescaler = 76 para freq 4Hz, logo escolher o 256,
                         // que é TCKPS = 3
    PR1 = 19499; // Fout = 20000KHz/256 = 78KHz
                 // PR1 = (78*10^3/4)Hz - 1 = 19499
    TMR1 = 0; // Reset timer T1 count register
    T1CONbits.TON = 1; // Enable timer T1 (must be the last command of the
                       // timer configuration sequence)

    // config timer T1 for interruptions
    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag
    
    /* T3 */
    // config timer T3 with freq = 100Hz  
    T3CONbits.TCKPS = 2; // Kprescaler = 3, logo escolher o 4
    PR3 = 49999; // Fout = 20000KHz/4 = 5000KHz
                 // PR3 = (5000*10^3/100)Hz - 1 = 49999
    TMR3 = 0;
    T3CONbits.TON = 1;
    OC1CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL = 1; // Use timer T3 as the time base for PWM generation
    OC1CONbits.ON = 1; // Enable OC1 module

    // config timer T3 for interruptions
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;

    // Configure UART1
    UART1config(115200,'N',1);

    EnableInterrupts();

    // config switches
    TRISBbits.TRISB0 = 1; // RB0-RB1 inputs
    TRISBbits.TRISB1 = 1; // 

    while(1) {
        int portVal = PORTB & 0x0003;
        switch(portVal) {
            case 0: // Measure input voltage
                IEC0bits.T1IE = 1; // Enable timer T1 interrupts
                setPWM(0);
                break;
            case 1: // Freeze
                IEC0bits.T1IE = 0; // Disable timer T1 interrupts
                setPWM(100);
                break;
            default:
                IEC0bits.T1IE = 1; // Enable timer T1 interrupts
                setPWM(voltage*3);
                break;
        }
    }

    return 0;
}

void setPWM(unsigned int dutyCycle) {
    if (dutyCycle >= 0 && dutyCycle <= 100) {
        OC1RS = ((PR3 + 1)*dutyCycle)/100;
    }
}

void putc(char byte2send) {
    while (U1STAbits.UTXBF == 1);
    U1TXREG = byte2send;
}

void puts(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        putc(str[i++]);
    }
}

void UART1config(int baudrate, char parity, int stop_bits) {
    U1MODEbits.BRGH = 0;
    U1BRG = (baudrate >= 600 && baudrate <= 115200) ? (PBCLK + 8 * baudrate) / (16 * baudrate) - 1 : (PBCLK + 8 * 115200) / (16 * 115200) - 1;
    U1MODEbits.STSEL = stop_bits == 2 ? 1 : 0;
    switch (parity) {
        case 'e':
        case 'E':
            U1MODEbits.PDSEL = 1;
            break;
        case 'o':
        case 'O':
            U1MODEbits.PDSEL = 10;
            break;
        default:
            U1MODEbits.PDSEL = 0;
    }
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    //RX interrupt enabled
    IPC6bits.U1IP = 2;		// configure priority of interrupts
	IEC0bits.U1RXIE = 1;	// enable interrupts
    U1MODEbits.ON = 1;
}

// Interrupt Handlers

void _int_(4) isr_T1(void) {
    AD1CON1bits.ASAM = 1; // start convertion
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag
}

void _int_(12) isr_T3(void) {
    static int count = 0;
    send2displays(toBcd(voltage), 1);
    if (++count == 100) {
        count = 0;
        putc(0x30 + (voltage >> 4));
		putc('.');
		putc(0x30 + (voltage & 0x0F));
		putc('\n');
    }
    IFS0bits.T3IF = 0;
}

void _int_(24) isr_uart1(void) {
    if (U1RXREG == 'L') {
        puts("voltMin = ");
        putc(0x30 + (voltMin >> 4));
		putc('.');
		putc(0x30 + (voltMin & 0x0F));
		putc('\n');

        puts("voltMax = ");
        putc(0x30 + (voltMax >> 4));
		putc('.');
		putc(0x30 + (voltMax & 0x0F));
		putc('\n');
    }
    // Clear UART1 rx interrupt flag
    IFS0bits.U1RXIF = 0;
}

void _int_(27) isr_adc(void) { // 27 is the vector number of A/D ()
                               // "PIC32 family datasheet" (pages 74-76)
    // ISR actions
    int sum=0;

    int *p = (int *)(&ADC1BUF0);
    for (; p <= (int *)(&ADC1BUFF); p+=4) {
        sum += *p;
    }
    voltage = ((sum/4*33)/1023);

    if (voltMin > voltage)
        voltMin = voltage;
    else if (voltMax < voltage)
        voltMax = voltage;

    IFS1bits.AD1IF = 0; // manual reset of AD1IF flag
}
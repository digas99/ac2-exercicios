#include <detpic32.h>

typedef struct {
    char mem[100];
    int nchar;
    int posrd;
} t_buf;

void setupADC(unsigned int );
void send2displays(unsigned char );
unsigned char toBcd(unsigned char );
void configUart2(unsigned int , char , unsigned int );
void putstrInt(char *, t_buf *);
void putcInt(char , t_buf *);
char intToAscii(int );

volatile t_buf txbuf;
volatile t_buf txbufVoltMin;
volatile t_buf txbufVoltMax;
volatile int voltage = 0;
volatile int voltMin = 0;
volatile int voltMax = 0;
int n_samples = 8;

int main(void) {
    TRISB = TRISB & (0x80FF); // set displays as outputs
    TRISDbits.TRISD5 = 0; // set RD6-5 as outputs
    TRISDbits.TRISD6 = 0;

    // Timer 1 config
    // Kpresc = PBCLK/(65536*5) = 61 -> 64
    PR1 = PBCLK/64/5-1;
    T1CONbits.TCKPS = 2;
    TMR1 = 0;
    T1CONbits.TON = 1;
    // Timer 1 interrupts
    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    IFS0bits.T1IF = 0;

    // Timer 3 config
    // Kpresc = PBCLK/(65536*100) = 3,05 -> 4
    PR3 = PBCLK/4/100-1;
    T3CONbits.TCKPS = 2;
    TMR3 = 0;
    T3CONbits.TON = 1;
    // Timer 3 interrupts
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0; 

    // ADC config
    setupADC(n_samples);
    // ADC interrupts
    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;
    IFS1bits.AD1IF = 0;

    // UART2 config
    configUart2(115200, 'N', 1);
    // UART2 Rx interrupts
    IEC1bits.U2TXIE = 1;
    IEC1bits.U2RXIE = 1;
    IPC8bits.U2IP = 2;
    IFS1bits.U2RXIF = 0;
    IFS1bits.U2TXIF = 0;
    U2STAbits.URXISEL = 0b00;
    
    EnableInterrupts();

    while(1);

    return 0; 
}

void _int_(4) isr_T1(void) {
    AD1CON1bits.ASAM = 1;

    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void) {
    send2displays(voltage);

    IFS0bits.T3IF = 0;
}

char intToAscii(int n) {
    return '0' + n;
}

void _int_(27) isr_adc(void) {
    int *p, sum = 0, i, avg, value;

    p = (int *)(&ADC1BUF0);
    for (i = 0; i < n_samples; i++) {
        value = p[i*4];
        sum += value;

        if (i == 0) voltMin = value;

        if (value < voltMin) voltMin = value;
        if (value > voltMax) voltMax = value;
    }

    avg = sum/n_samples;
    voltage = toBcd((avg*33+511)/1023);
    
    voltMin = toBcd((voltMin*33+511)/1023);
    txbufVoltMin.nchar = 0;
    putstrInt("VMin=", (t_buf *)&txbufVoltMin);
    putcInt(intToAscii(voltMin >> 4), (t_buf *)&txbufVoltMin);
    putcInt('.', (t_buf *)&txbufVoltMin);
    putcInt(intToAscii(voltMin & 0x0F), (t_buf *)&txbufVoltMin);
    putstrInt("V\n", (t_buf *)&txbufVoltMin);

    voltMax = toBcd((voltMax*33+511)/1023);
    txbufVoltMax.nchar = 0;
    putstrInt("VMax=", (t_buf *)&txbufVoltMax);
    putcInt(intToAscii(voltMax >> 4), (t_buf *)&txbufVoltMax);
    putcInt('.', (t_buf *)&txbufVoltMax);
    putcInt(intToAscii(voltMax & 0x0F), (t_buf *)&txbufVoltMax);
    putstrInt("V\n", (t_buf *)&txbufVoltMax);

    IFS1bits.AD1IF = 0;
}

void _int_(32) isr_uart2(void) {
    // this interrupt is triggered when the reception FIFO has atleast one new character to be read
    if (IFS1bits.U2RXIF == 1) {
        char c = U2RXREG;
        switch(c) {
            case 'M':
                txbuf = txbufVoltMax;
                break;
            case 'm':
                txbuf = txbufVoltMin;
                break;
        }

        IFS1bits.U2RXIF = 0;
    }

    // this interrupt is triggered when the transmition FIFO has at least one free slot
    if (IFS1bits.U2TXIF == 1) {
        if (txbuf.nchar > 0) {
            U2TXREG = txbuf.mem[txbuf.posrd++];
            txbuf.nchar--;
        }
        else {
            IEC1bits.U2TXIE = 0;
        }       

        IFS1bits.U2TXIF = 0;
    }
}

void setupADC(unsigned int samples) {
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = samples-1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;
}

void send2displays(unsigned char value) {
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;
    
    if (!displayFlag) {
        LATDbits.LATD6 = 0;
        LATDbits.LATD5 = 1;
        LATB = (LATB & 0x80FF) | (display7Scodes[(value & 0x0F)] << 8);
    }
    else {
        LATDbits.LATD6 = 1;
        LATDbits.LATD5 = 0;
        LATB = (LATB & 0x80FF) | (display7Scodes[(value >> 4)] << 8);    
    }

    displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}

void configUart2(unsigned int baud, char parity, unsigned int stopbits) {
    baud = baud == 600 || baud == 115200 ? baud : 115200;
    U2BRG = ((PBCLK+8*baud) / (16*baud)) - 1;
    U2MODEbits.BRGH = 0;
    
    switch(parity) {
        case 'e':
            U2MODEbits.PDSEL = 0b01;
            break;
        case 'o':
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

void putstrInt(char *s, t_buf *txbuf) {
    while(*s != '\0') {
        txbuf->mem[txbuf->nchar++] = *s;
        s++;
    }
    txbuf->posrd = 0;
    IEC1bits.U2TXIE = 1;
}

void putcInt(char c, t_buf *txbuf) {
    txbuf->mem[txbuf->nchar++] = c;
    
    txbuf->posrd = 0;
    IEC1bits.U2TXIE = 1;
}
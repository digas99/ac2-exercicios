#include <detpic32.h>

// 1.
#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1

// 2.
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)

typedef struct
{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer;

volatile circularBuffer txb; // Transmission buffer
volatile circularBuffer rxb; // Reception buffer

// 3.
void comDrv_flushRx(void) {
    rxb.count = 0;
    rxb.tail = 0;
    rxb.head = 0;
}

void comDrv_flushTx(void) {
    txb.count = 0;
    txb.tail = 0;
    txb.head = 0;
}

// 4.
void comDrv_putc(char ch) {
    while (txb.count >= BUF_SIZE); // Wait while buffer is full
    txb.data[txb.tail] = ch; // Copy character to the transmission buffer at
                             // position tail
    txb.tail = (txb.tail + 1) & INDEX_MASK; // Increment "tail" index (mod. BUF_SIZE)
    DisableUart1TxInterrupt(); // Begin of critical section
    txb.count++;
    EnableUart1TxInterrupt(); // End of critical section
}

// 5.
void comDrv_puts(char *s) {
    int i = 0;
    while (s[i] != '\0') {
        comDrv_putc(s[i++]);
    }
}

// 8.
char comDrv_getc(char *pchar) {
    if (rxb.count == 0) return 0; // Test "count" variable (reception buffer) and 
                                  // return FALSE if it is zero
    DisableUart1TxInterrupt(); // Begin of critical section
    *pchar = rxb.data[rxb.head];
    rxb.count--; 
    rxb.head = (rxb.head + 1) & INDEX_MASK; // Increment "head" index (mod. BUF_SIZE)
    EnableUart1TxInterrupt(); // End of critical section
    return 1;
}

// 6. & 9.
void _int_(24) isr_uart1(void) {
    // 6.
    if (IFS0bits.U1TXIF == 1) {
        if (txb.count > 0) {
            U1TXREG = txb.data[txb.head];
            txb.head = (txb.head + 1) & INDEX_MASK;
            txb.count--;
        }
        else {
            DisableUart1TxInterrupt();
        }
        IFS0bits.U1TXIF = 0;
    }

    // 9.
    if (IFS0bits.U1RXIF == 1) {
        rxb.data[rxb.tail] = U1RXREG; // Read character from UART and write it to
                                      // the "tail" position of the reception buffer
        rxb.tail = (rxb.tail + 1) & INDEX_MASK; // Increment "tail" index (mod. BUF_SIZE)
        if (rxb.count < BUF_SIZE) {
            rxb.count++;
        }
        else {
            rxb.head = (rxb.head + 1) & INDEX_MASK;
        }
        IFS0bits.U1RXIF = 0;
    }
}

// 7.
void comDrv_config(int baudrate, char parity, int stop_bits) {
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

    U1MODEbits.ON = 1;

    // config interrupts
    IFS0bits.U1RXIF = 0;	// clear interrupt flag (RX)
	IFS0bits.U1TXIF = 0;	// clear interrupt flag (TX)
    IPC6bits.U1IP = 2;		// configure priority of interrupts
}

// 7. & 10. & 11.
int main(void) {
    comDrv_config(115200, 'N', 1);
    comDrv_flushRx();
    comDrv_flushTx();

    EnableInterrupts();
    EnableUart1RxInterrupt();

    comDrv_puts("PIC32 UART Device-driver\n");
    char rd;
    while (1) {
        // Read character from reception buffer
        if (comDrv_getc(&rd)) {
            if (rd == 'S') {
                comDrv_puts("\n[Detected character 'S' from reception buffer]\n");
            }
            else {
                // Send character to the transmission buffer
                comDrv_putc(rd);
            }
        }
    }

    return 0;
}
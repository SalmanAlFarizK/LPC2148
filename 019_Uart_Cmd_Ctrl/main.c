#include <LPC21XX.h>
#include <stdio.h>
#include <string.h>

#define LED (1 << 3)

void delay(unsigned int n) {
    unsigned int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 500; j++);
}

void UART_Init() {
    PINSEL0 |= 0x05;      // P0.0 (TxD) and P0.1 (RxD)
    U0LCR = 0x83;         // Enable DLAB
    U0DLL = 97;           // Baud rate 9600 for 15MHz PCLK
    U0DLM = 0x00;
    U0LCR = 0x03;         // 8-bit data, 1 stop bit
}

void UART_Tx(char data) {
    while (!(U0LSR & 0x20));
    U0THR = data;
}

void UART_data_Tx(char *str) {
    while (*str) {
        UART_Tx(*str++);
    }
}

char UART_Rx() {
    while (!(U0LSR & 0x01));  // Wait until data is received
    return U0RBR;
}

void UART_Rx_data(char *buffer) {
    char ch;
    int i = 0;
    while (1) {
        ch = UART_Rx();
        UART_Tx(ch);  // Echo the character
        if (ch == '\r') {
            buffer[i] = '\0';
            UART_Tx('\n');
            break;
        } else {
            buffer[i++] = ch;
        }
    }
}

int main() {
    char rxBuffer[20];

    UART_Init();

    // Configure LED pin
    IODIR0 |= LED;  // P1.25 as output
    IOCLR0 = LED;   // LED OFF initially
		UART_data_Tx("Enter Command:\r\n");
    while (1) {
        // Check if a command is received
        if (U0LSR & 0x01) {  // Data available 
						
            UART_Rx_data(rxBuffer);
            if (strcmp(rxBuffer, "LEDON") == 0) {
                IOSET0 = LED;
							UART_data_Tx("Enter Command:\r\n");
            } 
						else if (strcmp(rxBuffer, "LEDOFF") == 0) {
                IOCLR0 = LED;
							UART_data_Tx("Enter Command:\r\n");
						}
						else {
              UART_data_Tx("Invalid Command\r\n");
							UART_data_Tx("Enter Command:\r\n");
            }
					}
    }
}

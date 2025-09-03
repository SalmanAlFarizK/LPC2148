#include <stdio.h>
#include <lpc214x.h>
#include <stdint.h>
#include <string.h>

void InitUart0(void);
void UartInterruptConfig(void);
__irq void Uart0ISR(void);
void UartSend(uint8_t* ucCharBuff, uint32_t uiLen);
void delay_ms(int ms);

volatile uint8_t iSUartInterrupt = 0;
volatile uint8_t receivedData = 0; // Buffer for received data

int main()
{
    uint8_t ucRxData = 0;
    uint8_t ucTxDataBuff[100] = {0};
    uint32_t uiTxDataIndex = 0;

    InitUart0();
   // __asm volatile ("cpsie i"); // Enable global interrupts

    while(1)
    {
        if(iSUartInterrupt)
        {
            ucRxData = receivedData; // Use data from ISR
            uiTxDataIndex = sprintf((char*)ucTxDataBuff, "Received: ");
            ucTxDataBuff[uiTxDataIndex] = ucRxData;
            ucTxDataBuff[uiTxDataIndex + 1] = '\r';
            ucTxDataBuff[uiTxDataIndex + 2] = '\n';

            UartSend(ucTxDataBuff, uiTxDataIndex + 3);
            iSUartInterrupt = 0;
        }
        else
        {
            uint8_t cStr[] = "Nothing received\r\n";
            UartSend(cStr, sizeof(cStr));
        }
        delay_ms(200);
    }
}

void InitUart0(void)
{
    VPBDIV = 0x01; // PCLK = CCLK = 60 MHz
    PINSEL0 |= 0x00000005; // P0.0 = TXD0, P0.1 = RXD0
    U0LCR = 0x83; // Enable DLAB, 8-bit, 1 stop, no parity
    // 9600 bps: PCLK / (16 * (U0DLM << 8 + U0DLL)) = 60M / (16 * 390) ˜ 9600
    U0DLL = 390 & 0xFF; // Low byte
    U0DLM = (390 >> 8) & 0xFF; // High byte
    U0LCR = 0x03; // Disable DLAB
    U0IER = (1 << 0); // Enable Receive Data Available interrupt
    UartInterruptConfig();
}

void UartInterruptConfig(void)
{
    VICIntEnClr = (1 << 6); // Disable UART0 interrupt
    VICVectCntl4 = (1 << 5) | 6; // Slot 4, UART0 (source 6)
    VICVectAddr4 = (unsigned long)Uart0ISR; // ISR address
    VICIntEnable = (1 << 6); // Enable UART0 interrupt
}

__irq void Uart0ISR(void)
{
    receivedData = U0RBR; // Read U0RBR to clear RDA interrupt
    iSUartInterrupt = 1; // Set flag
    VICVectAddr = 0x0; // Clear VIC interrupt
}

void UartSend(uint8_t* ucCharBuff, uint32_t uiLen)
{
    while(uiLen > 0)
    {
        while(!(U0LSR & 0x20)); // Wait for THR empty
        U0THR = *ucCharBuff++;
        uiLen--;
    }
}

void delay_ms(int ms)
{
	unsigned int i, j;
    for( i = 0; i < ms; i++)
	{
		for( j = 0; j < 6000; j++);
	}
}

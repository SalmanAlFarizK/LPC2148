#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


void Uart0Init(void)
{
	PINSEL0 &= ~0x0000000F;
	PINSEL0 |= 0x00000005;
	VPBDIV = 0x04; 
	
	U0LCR = 0x83;
	U0DLL = 97;
	U0DLM = 0;
  U0FDR = 0x10;              
  U0LCR = 0x03;
	
	U0FCR = 0x07;
}

uint32_t Uart0Rxdata(uint8_t* ucRxBuff, uint32_t uiSize)
{
	uint32_t uiRxIdx = 0;
	VPBDIV = 0x04; 
	
	while(uiSize > 0)
	{
		while(!(U0LSR & 0x01));
		
		ucRxBuff[uiRxIdx++] = U0RBR;
		
		uiSize -= 1; 
	}
	
	return uiRxIdx;
}

void UartSend(uint8_t* ucCharBuff, uint32_t uiLen)
{
	uint8_t ucTrackIdx = 0;
	while(uiLen > 0)
	{
		while(!(U0LSR & 0x20));
		U0THR = ucCharBuff[ucTrackIdx];
		ucTrackIdx += 1;
		uiLen -= 1;
	}
}

int main()
{
	uint8_t ucTxBuff[100] = {0};
	uint8_t ucRxBuff[100] = {0};
	uint32_t uiRxSize = 0;
  uint32_t uiTxBuffTracker = 0;

	Uart0Init();
	
	while(1)
	{
		uiRxSize = Uart0Rxdata(ucRxBuff, 1);
		
		if(uiRxSize > 0)
		{
			uiTxBuffTracker = sprintf((char*)ucTxBuff , "Received: ");
			
			memcpy(ucTxBuff + uiTxBuffTracker, ucRxBuff, uiRxSize);
			
      ucTxBuff[uiTxBuffTracker + uiRxSize] = '\r';
      ucTxBuff[uiTxBuffTracker + uiRxSize + 1] = '\n';
			
			UartSend(ucTxBuff, uiTxBuffTracker + uiRxSize + 2);
		}
	}
}

#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>
#include <stdint.h>

void delay_ms(uint16_t ms);
void Uart0Init(void);
void UartSend(uint8_t* ucCharBuff, uint32_t uiLen);

void InitDac(uint32_t uiVal);

int main()
{
	InitDac(1000);
	Uart0Init();
	
	while(1)
	{
		InitDac(50);
		delay_ms(200);
		InitDac(500);
		delay_ms(200);
	}
}

void delay_ms(uint16_t ms)
{
	unsigned int i, j;
	
	for(i = 0; i < ms; ++i)
	{
		for(j = 0; j < 6000; ++j);
	}
}

void Uart0Init(void)
{
	PINSEL0 |= 0x00000005;
	U0LCR = 0x83;
	U0DLL = 391 & 0xFF;         // Low byte
	U0DLM = (391 >> 8) & 0xFF;  // High byte
	U0LCR = 0x03;
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

void InitDac(uint32_t uiVal)
{
	uint32_t uiDigitalData = uiVal;
	
	PINSEL1 |= ( 2 << 18);
	DACR = (uiDigitalData << 6);
}


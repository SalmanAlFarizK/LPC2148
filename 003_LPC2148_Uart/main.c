#include <lpc214x.h>
#include <string.h>
#include <stdint.h>

void Uart0Init(void)
{
	PINSEL0 |= 0x00000005;
	U0LCR = 0x83;
	U0DLL = 97;
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

void delay(int ms)
{
	unsigned int i, j;
	for(i = 0; i < ms; ++i)
	{
		for(j = 0; j < 7500; ++j)
		{
			
		}
	}
}

int main()
{
	char cStr[] = "Hello World\n";
	uint32_t uiSize = sizeof(cStr);
	
	Uart0Init();
	while(1)
	{
		UartSend((uint8_t*)cStr,uiSize);
		delay(100);
	}
}

#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void Uart0Init(void);
void UartSend(uint8_t* ucCharBuff, uint32_t uiLen);
void delay_ms(unsigned int ms);

int main()
{
	uint32_t uiBtnPressCnt = 0;
	uint8_t ucBtnPressFlag = 0;
	uint8_t ucTxBuff[100] = {0};
	uint32_t uiTxBffIdx = 0;
	
	IO0DIR &= ~(1 << 3);  // P0.3 as input (Switch)
	Uart0Init();
	
	while(1)
	{
		if(ucBtnPressFlag)
		{
			uiTxBffIdx = sprintf((char*)ucTxBuff, "Button pressed %d times\r\n", uiBtnPressCnt);
			
			UartSend(ucTxBuff, uiTxBffIdx);
			ucBtnPressFlag = 0;
		}
		
    if(!(IO0PIN & (1 << 3)) == 0)   // If switch is not pressed
    {
			uiBtnPressCnt += 1;
			ucBtnPressFlag = 1;
    }
		
		delay_ms(100);
	}
}

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

void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	
	for(i = 0; i < ms; ++i)
	{
		for(j = 0; j < 6000; ++j);
	}
}

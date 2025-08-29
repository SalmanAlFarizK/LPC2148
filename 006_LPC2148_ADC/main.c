#include <lpc214x.h>
#include <stdint.h>
#include <string.h>


void Delay(int ms)
{
	unsigned int i, j;
	
	for(i = 0; i < ms; ++i)
	{
		for(j = 0; j < 7500; ++j);
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

void ADC0Init(void)
{
	PINSEL1 &= ~( 1 << 29);
	PINSEL1 |= ( 1 << 28);
	
	//Peripheral control power enable.
	PCONP |= ( 1 << 12 );
	
	// Select channel (here 3rd channel)
	AD0CR |= ( 1 << 3);
	
	//Select ADC clock as VPB or (PCLK/8).
	AD0CR |= (0x00000700);
	
	//Disable burst mode.
	AD0CR &= (0xFFFEFFFF); 
	
	//Set resolution as 10 bit 11 clock cycles.
	AD0CR &= (0xFFF1FFFF);
	
	//Activate ADC module. ( Enable PDN bit)
	AD0CR |= ( 1 << 21 );
}

uint32_t GetAdcVal(void)
{
	uint32_t uiAdcVal = 0;
	
	//Satrt ADC conversion.
	AD0CR |= 0x01000000;
	
	// Wait ADC conversion to complete.
	while((AD0GDR & 0x80000000) == 0);
	
	// Read the ADC value.
	uiAdcVal = ((AD0DR3 >> 6) & 0x000003FF);
	
	//Stop ADC conversion.
	AD0CR &= 0xF8FFFFFF;
	
	return uiAdcVal;
}

int main()
{
	uint32_t uiAdcVal = 0;
	
	Uart0Init();
	
}
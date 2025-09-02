#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>


void delay_ms(uint16_t ms)
{
	unsigned int i, j;
	
	for(i = 0; i < ms; ++i)
	{
		for(j = 0; j < 6000; ++j);
	}
}

int main()
{
	PINSEL0 = 0x00000000;
	IODIR0 = 0xFFFFFFFF;
	
	while(1)
	{
		IOPIN0 = 0xFC;
		delay_ms(500);
		
		IOPIN0 = 0x60;
		delay_ms(500);
		
		IOPIN0 = 0xDA;
		delay_ms(500);
		
		IOPIN0 = 0xF2;
		delay_ms(500);
		
		IOPIN0 = 0x66;
		delay_ms(500);
		
		IOPIN0 = 0xB6;
		delay_ms(500);
		
		IOPIN0 = 0xBE;
		delay_ms(500);
		
		IOPIN0 = 0xE0;
		delay_ms(500);
	}
}
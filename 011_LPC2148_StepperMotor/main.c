#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>


void delay_ms(uint16_t ms)
{
	unsigned int i, j;
	
	for(i = 0; i < ms; ++i)
	{
		for(j = 0; j < 60000; ++j);
	}
}

int main()
{
	IO0DIR |= 0x0000000F;
	
	while(1)
	{
		IO0PIN = (1 << 0);
		delay_ms(500);
		
		IO0PIN = (1 << 1);
		delay_ms(500);
		
		IO0PIN = (1 << 2);
		delay_ms(500);
		
		IO0PIN = (1 << 3);
		delay_ms(500);
		
		IO0PIN = (1 << 3);
		delay_ms(500);
		
		IO0PIN = (1 << 2);
		delay_ms(500);
		
		IO0PIN = (1 << 1);
		delay_ms(500);
		
		IO0PIN = (1 << 0);
		delay_ms(500);
	}
}

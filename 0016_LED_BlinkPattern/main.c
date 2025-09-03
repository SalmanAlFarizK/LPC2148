#include <stdio.h>
#include <lpc214x.h>

void SetPinHigh(unsigned int PinNo)
{
	
	/* Clear rest all other pins. */
	IO0CLR  = 0x000000FF;
	
	/* Set the slected pin. */
	IO0SET = (1 << PinNo);
}

void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	
	for(i = 0; i < ms; ++i)
	{
		for(j = 0; j < 6000; ++j);
	}
}

int main()
{
	unsigned int i;
	
	IO0DIR |= 0x000000FF;
	
	while(1)
	{
		for(i = 0; i < 8; ++i)
		{
			SetPinHigh(i);
			delay_ms(100);
		}
	}
}

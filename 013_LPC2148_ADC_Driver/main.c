#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>

#define END_OF_ADC1_CONVERSION (AD1GDR & ( 1UL << 31))

void InintAdc(void);
unsigned int AcquireAdcVal(void);
void delay_ms(uint16_t ms);
void Uart0Init(void);
void UartSend(uint8_t* ucCharBuff, uint32_t uiLen);

int main()
{
	uint8_t ucTxBuff[100] = {0};
	uint32_t uiTxSize = 0;
	unsigned int uiAdcVal = 0;
	
	Uart0Init();
	InintAdc();
	
	while(1)
	{
		uiAdcVal = AcquireAdcVal();
		
		if(uiAdcVal > 0)
		{
			uiTxSize = sprintf((char*)ucTxBuff, "The ADC value received is : %d\r\n",uiAdcVal);
			
			UartSend(ucTxBuff, uiTxSize);
		}
		else
		{
			uiTxSize = sprintf((char*)ucTxBuff, "No ADC Value is received\r\n");
			
			UartSend((uint8_t*)ucTxBuff, uiTxSize);
		}
		
		delay_ms(100);
	}
}

void InintAdc(void)
{
	PINSEL1 |= (1 << 12);
	PINSEL1 &= ~(1 << 13);
	
	AD1CR = 0x00000000;
	
	/* Channel 7 selection. */
	AD1CR |= ( 1 << 7);
	
	/* Write 14 for ADC Clk 4.5 MHz. */
	AD1CR |= ( 13 << 8);
	
	/* Set Busrt bit, (Burst = 1). */
	AD1CR |= ( 1 << 16);
	
	/* Enable ADC 1. */
	AD1CR |= ( 1 << 21);
}

unsigned int AcquireAdcVal(void)
{
	unsigned int uiAdcVal = 0;
	
	/* Start ADC conversion. */
	AD1CR |= ( 1 << 24);
	
	/* Wait until the end of adc conversion. */
	while(!(END_OF_ADC1_CONVERSION));
	
	/* Read the ADC value. */
	uiAdcVal = ((AD1GDR >> 6) & 0x000003FF);
	
	/* Stop the ADC conversion. */
	AD1CR &= ~(1 << 24);
	
	/* Return the adc value. */
	return uiAdcVal;
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


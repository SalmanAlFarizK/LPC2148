#include <stdio.h>
#include <lpc214x.h>
#include <string.h>
#include <stdint.h>

void pwm_init(void);
void Uart0Init(void);
void delay(int ms);
void UartSend(uint8_t* ucCharBuff, uint32_t uiLen);

unsigned int duty_cycle = 75;

int main()
{
	uint8_t cTxBuff[25] = {0};
	uint32_t uiTxSize = 0;

	pwm_init();
	Uart0Init();
	
	while(1)
	{
		uiTxSize = sprintf((char*)cTxBuff, "Duty Cycle %d %\r\n",duty_cycle);
		UartSend(cTxBuff, uiTxSize);
		delay(200);
	}
}

void pwm_init(void) {
    PINSEL1 |= 0x00000400; // Enable PWM5 on P0.21
		PWMPCR = 0x00;				 //Single edge PWM mode 
    PWMPR = 60000 - 1;     // Presclar for 1ms, as PCLK = 60MHz = CCLK
    PWMMR0 = 10;          // time period 10ms

    PWMMCR = (1<<1);  		 // Reset TC on MR0 match
    PWMLER = (1<<5) | (1<<0); // Enable updates to MR0 and MR1
    PWMPCR = (1<<13);   // Enable PWM output for PWM1
		PWMTCR = (1<<1);		//Reset TC 
    PWMTCR = (1<<0) | (1<<3);   // Enable PWM and Counter
	
		PWMMR5 = duty_cycle/10;       // duty cycle 
	  PWMLER = (1<<5);
}


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

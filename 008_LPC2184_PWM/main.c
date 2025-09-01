#include <stdio.h>
#include <lpc214x.h>
#include <string.h>

void InitPwm(void);
void pwm_init(void);

unsigned int duty_cycle = 75;

int main()
{
	pwm_init();
	
	while(1);
}

void InitPwm(void)
{
    // Configure P0.21 as PWM5
    PINSEL1 |= (1 << 10);   
    
    // Single edge mode
    PWMPCR = 0x00;
    
    // Prescaler: 1 ms tick at 60 MHz
    PWMPR = 60000 - 1;
    
    // Period = 10 ms
    PWMMR0 = 10;
    
    // Duty cycle = 1 ms (10%)
    PWMMR5 = 1;
    
    // Reset on MR0
    PWMMCR = (1 << 1);
    
    // Latch enable
    PWMLER = (1 << 0) | (1 << 5);
    
    // Enable PWM5 output
    PWMPCR = (1 << 13);
    
    // Reset counter
    PWMTCR = (1 << 1);
    
    // Enable counter and PWM mode
    PWMTCR = (1 << 0) | (1 << 3);
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


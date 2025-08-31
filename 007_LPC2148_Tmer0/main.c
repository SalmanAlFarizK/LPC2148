#include <stdio.h>
#include <lpc214x.h>
#include <string.h>
#include <stdint.h>


#define PRESCALE 	60000
#define DELAY_MS 	1
#define MR0I			( 1 << 0 )
#define MR0R			( 1 << 1 )


void InitTim0(void);
__irq void T0ISR(void);
void Uart0Init(void);
void UartSend(uint8_t* ucCharBuff, uint32_t uiLen);
void Send5SecMessage(void);

typedef struct _SysClk_
{
	long MilliSec;
	short Day;
} SysClk;

SysClk gtSysClk;
uint32_t uiLastMsgSendTime = 0;
uint32_t uiSndMsgFlag = 0;

void delay(int ms)
{
	int i, j;
	for(i = 0; i < ms; ++i)
	{
		for(j = 0 ; j < 7500; ++j);
	}
}

int main()
{
	//char cStr[] = "Hello world\n";
	
	InitTim0();
	Uart0Init();
	
	while(1)
	{
		if(uiSndMsgFlag)
		{
			Send5SecMessage();
			uiSndMsgFlag = 0;
		}
	}
}

void InitTim0(void)
{
    T0TCR = 0x02;                  // Reset timer
    T0CTCR = 0x00;              // Timer mode (very important!)
    T0PR  = 59999;                 // Prescaler: 60MHz/60000 = 1kHz
    T0MR0 = 1;                  // Match at 1000 ticks = 1ms
    T0MCR = (MR0I | MR0R);         // Interrupt + Reset
    VICVectAddr4 = (unsigned)T0ISR;
    VICVectCntl4 = 0x20 | 4;       // Slot enable, Timer0 IRQ = 4
    VICIntEnable |= (1 << 4);      // Enable Timer0 interrupt
    T0TCR = 0x01;                  // Start timer
}

void Uart0Init(void)
{
	PINSEL0 &= ~0x0000000F;
	PINSEL0 |= 0x00000005;
	VPBDIV = 0x01; 
	
	U0LCR = 0x83;
	U0DLL = 134;
	U0DLM = 0x01;
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

void Send5SecMessage(void)
{
	uint8_t ucMsgBuff[100] = {0};
  uint32_t uiTxBuffSize = 0;

	uiTxBuffSize = sprintf((char*)ucMsgBuff, "The OBC Time is : %d \r\n",gtSysClk.MilliSec);
	UartSend(ucMsgBuff, uiTxBuffSize);

}

__irq void T0ISR(void)
{
	long int iRegVal;
	
	gtSysClk.MilliSec += 1;
	
	if((gtSysClk.MilliSec - uiLastMsgSendTime) > 5000)
	{
		uiSndMsgFlag = 1;
		uiLastMsgSendTime = gtSysClk.MilliSec;
	}
	
	iRegVal = T0IR;
	T0IR = iRegVal;
	VICVectAddr = 0x0;
}

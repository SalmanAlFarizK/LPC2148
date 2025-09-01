#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define LCD ( 0xFF << 8)
#define RS  ( 1 << 16)
#define RW  ( 1 << 17)
#define EN  ( 1 << 18)

#define COL1	( 1 << 16 )
#define COL2	( 1 << 17 )
#define COL3	( 1 << 18 )
#define COL4	( 1 << 19 )
#define ROW1	( 1 << 20 )
#define ROW2	( 1 << 21 )
#define ROW3	( 1 << 22 )
#define ROW4	( 1 << 23 )

void InitLcd(void);
void Delay(uint16_t uhMs);
void LCD_CMD(char cmd);
void LCD_STRING(char* msg);
void LCD_CHAR(char msg);
void LCD_DAT(unsigned int data);
void LCD_PRINT(char* str);
unsigned char KeyPad(void);
void KeyPadDElay(void);

char val_j[3];
int main()
{
	PINSEL0 = 0x00000000;
	IODIR0 = 0xFFFFFFFF;
	PINSEL1 = 0x00000000;
	IODIR1 = 0x00F00000;
	
	InitLcd();
	
	LCD_PRINT("Press any Key");
	LCD_CMD(0xc0);
	
	while(1)
	{
		LCD_DAT(KeyPad());
		KeyPadDElay();
	}
}

void InitLcd(void)
{
	Delay(5);
	
	LCD_CMD(0x38);
	LCD_CMD(0x0F);
	LCD_CMD(0x06);
	LCD_CMD(0x01);
	Delay(5);
	LCD_CMD(0x80);
}

void Delay(uint16_t uhMs)
{
	int i , j;
	
	for(i = 0; i < uhMs; ++i)
	{
		for(j = 0; j < 60000; ++j);
	}
}

void LCD_CMD(char cmd)
{
	IO0CLR = (RS | RW | EN | LCD);
	IO0SET = ( cmd << 8);
	IO0CLR |= RS;
	IO0CLR |= RW;
	IO0SET |= EN;
	Delay(2);
	IO0CLR |= EN;
	Delay(3);
}

void LCD_DAT(unsigned int data)
{
    IO0CLR = (RS | RW | EN | LCD);
    IO0SET = (data << 8);
    IO0SET |= RS;    // ? SET RS for data mode
    IO0CLR |= RW;    // Write mode
    IO0SET |= EN;    // Enable pulse
    Delay(2);
    IO0CLR |= EN;
    Delay(3);
}


void LCD_STRING(char* msg)
{
	uint8_t i = 0;
	
	while(msg[i] != 0)
	{
		IO0PIN = ((IO0PIN & 0xFFFF00FF) | ((msg[i] & 0xF0) << 8 ));
		IO0SET = 0x00000050;
		IO0CLR = 0x00000020;
		
		Delay(2);
		IO0CLR = 0x00000040;
		Delay(5);
		IO0PIN = ((IO0PIN & 0xFFFF00FF) | ((msg[i] & 0x0F) << 12 ));
		IO0SET = 0x00000050;
		IO0CLR = 0x00000020;
		
		Delay(2);
		IO0CLR = 0x00000040;
		Delay(5);
		i++;
	}
}

void LCD_CHAR(char msg)
{
	IO0PIN = ((IO0PIN & 0xFFFF00FF) | ((msg & 0xF0) << 8 ));
	IO0SET = 0x00000050;
	IO0CLR = 0x00000020;	
	Delay(2);
	
	IO0CLR = 0x00000040;
	Delay(5);
	
	IO0PIN = ((IO0PIN & 0xFFFF00FF) | ((msg & 0x0F) << 12 ));
	IO0SET = 0x00000050;
	IO0CLR = 0x00000020;
	Delay(2);
		
	IO0CLR = 0x00000040;
	Delay(5);
}

void LCD_PRINT(char* str)
{
	while(*str != '\0')
	{
		LCD_DAT(*str);
		str++;
	}
}

void KeyPadDElay(void)
{
	unsigned int t1, t2;
	
	for(t1 = 0; t1 < 300; ++t1)
	{
		for(t2 = 0; t2 < 1275; ++t2);
	}
}

unsigned char KeyPad(void)
{
	unsigned char key;
	IOCLR1 |= (ROW1 | ROW2 |ROW3 | ROW4 | COL1 | COL2 | COL3 | COL4);
	
	while(1)
	{
		if((IOPIN1 & COL1) == 0)
		{
			key = '7';
			KeyPadDElay();
			return key;
		}
		else if((IOPIN1 & COL2) == 0)
		{
			key = '8';
			KeyPadDElay();
			return key;
		}
		else if((IOPIN1 & COL3) == 0)
		{
			key = '9';
			KeyPadDElay();
			return key;
		}
		else if((IOPIN1 & COL4) == 0)
		{
			key = '/';
			KeyPadDElay();
			return key;
		}
		
		IOCLR1 |= ROW2;
		IOSET1 |= (ROW1 | ROW3 | ROW4);
		
		if((IOPIN1 & COL1) == 0)
		{
			key = '4';
			KeyPadDElay();
			return key;
		}
		else if((IOPIN1 & COL2) == 0)
		{
			key = '4';
			KeyPadDElay();
			return key;
		}
		else if((IOPIN1 & COL3) == 0)
		{
			key = '6';
			KeyPadDElay();
			return key;
		}
		else if((IOPIN1 & COL4) == 0)
		{
			key = '*';
			KeyPadDElay();
			return key;
		}
		
		IOCLR1 |= ROW3;
		IOSET1 |= (ROW1 | ROW2 | ROW4);
		
		if((IOPIN1 & COL1) == 0)
		{
			key = '1';
			KeyPadDElay();
			return key;
		}
		else if((IOPIN1 & COL2) == 0)
		{
			key = '2';
			KeyPadDElay();
			return key;
		}
		else if((IOPIN1 & COL3) == 0)
		{
			key = '3';
			KeyPadDElay();
			return key;
		}
		else if((IOPIN1 & COL4) == 0)
		{
			key = '-';
			KeyPadDElay();
			return key;
		}
		
		IOCLR1 |= ROW4;
		IOSET1 |= (ROW1 | ROW2 | ROW3);
		
		if((IOPIN1 & COL1) == 0)
		{
			LCD_CMD(0x01);
			KeyPadDElay();
		}
		else if((IOPIN1 & COL2) == 0)
		{
			key = '0';
			KeyPadDElay();
			return key;
		}
		else if((IOPIN1 & COL3) == 0)
		{
			key = '=';
			KeyPadDElay();
			return key;
		}
		else if((IOPIN1 & COL4) == 0)
		{
			key = '+';
			KeyPadDElay();
			return key;
		}
	}
}

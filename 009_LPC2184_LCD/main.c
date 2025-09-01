#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void InitLcd(void);
void Delay(uint16_t uhMs);
void LCD_CMD(char cmd);
void LCD_STRING(char* msg);
void LCD_CHAR(char msg);

char val_j[3];
int main()
{
	uint8_t j = 0;
	
	InitLcd();
	LCD_STRING("Good day");
	LCD_CMD(0xC0);
	
	for(j = 0; j < 10; ++j)
	{
		sprintf(val_j, "%d",j);
		LCD_STRING(val_j);
		Delay(100);
		LCD_CMD(0xCC);
	}
}

void InitLcd(void)
{
	IO0DIR = 0x0000FFF0;
	Delay(20);
	
	LCD_CMD(0x02);
	LCD_CMD(0x28);
	LCD_CMD(0x0C);
	LCD_CMD(0x06);
	LCD_CMD(0x01);
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
	IO0PIN = ((IO0PIN & 0xFFFF00FF) | ((cmd & 0xF0) << 8));
	IO0SET = 0x00000040;
	IO0CLR = 0x00000030;
	
	Delay(5);
	IO0CLR = 0x00000040;
	Delay(5);
	
	IO0PIN = (( IO0PIN & 0xFFFF00FF) | ((cmd & 0x0f) << 12));
	IO0SET = 0x00000040;
	IO0CLR = 0x00000030;
	Delay(5);
	
	IO0CLR = 0x00000040;
	Delay(5); 
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

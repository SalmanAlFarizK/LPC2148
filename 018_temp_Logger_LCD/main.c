#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define LCD (0xFF << 8)
#define RS  (1 << 16)
#define RW  (1 << 17)
#define EN  (1 << 18)

#define END_OF_ADC0_CONVERSION (AD0GDR & (1UL << 31))

void InintAdc(void);
unsigned int AcquireAdcVal(void);
void InitLcd(void);
void Delay(uint16_t uhMs);
void LCD_CMD(char cmd);
void LCD_STRING(char* msg);
void LCD_CHAR(char msg);
void LCD_DAT(unsigned int data);
void LCD_PRINT(char* str);

char val_j[3];

int main()
{	
    // Configure pins
    PINSEL0 = 0x00000000;  // All P0.0-P0.15 as GPIO (for LCD)
    PINSEL1 = 0x08000000;  // P0.27 as AD0.0 (bits 23:22 = 10), others as GPIO
    
    IODIR0 = 0xFFFFFFFF;   // All port 0 as outputs (for LCD)
    IODIR1 = 0x00F00000;   // Keypad rows as outputs
    
    InitLcd();
    InintAdc();  // Initialize ADC
    
    LCD_PRINT("Temp: ");
    LCD_CMD(0xc0);
    
    while(1)
    {
        unsigned int uiAdCVal = AcquireAdcVal();
        char cAdcStr[5] = {0};
        
        // Convert ADC value to temperature (assuming 10mV/°C)
        // For 3.3V reference and 10-bit ADC: 
        // Voltage = (ADC_value * 3.3) / 1024
        // Temperature = Voltage * 100
        unsigned int temperature = (uiAdCVal * 330) / 1024;
        
        sprintf(cAdcStr, "%d C", temperature);
        LCD_CMD(0xC6);  // Position cursor at column 6 of second line
        LCD_PRINT(cAdcStr);
        
        Delay(1000);    // Update every second
    }
}

void InitLcd(void)
{
    Delay(15);
    
    LCD_CMD(0x38);  // 8-bit mode, 2 lines, 5x7 font
    LCD_CMD(0x0C);  // Display on, cursor off, blink off
    LCD_CMD(0x06);  // Increment cursor, no shift
    LCD_CMD(0x01);  // Clear display
    Delay(5);
    LCD_CMD(0x80);  // Set cursor to first line first position
}

void Delay(uint16_t uhMs)
{
    int i, j;
    
    for(i = 0; i < uhMs; ++i)
    {
        for(j = 0; j < 6000; ++j);  // Increased for better timing
    }
}

void LCD_CMD(char cmd)
{
    IO0CLR = (RS | RW | EN | LCD);
    IO0SET = (cmd << 8);
    IO0CLR |= RS;   // Command mode
    IO0CLR |= RW;   // Write mode
    IO0SET |= EN;   // Enable pulse
    Delay(2);
    IO0CLR |= EN;
    Delay(3);
}

void LCD_DAT(unsigned int data)
{
    IO0CLR = (RS | RW | EN | LCD);
    IO0SET = (data << 8);
    IO0SET |= RS;    // Set RS for data mode
    IO0CLR |= RW;    // Write mode
    IO0SET |= EN;    // Enable pulse
    Delay(2);
    IO0CLR |= EN;
    Delay(3);
}

void LCD_PRINT(char* str)
{
    while(*str != '\0')
    {
        LCD_DAT(*str);
        str++;
    }
}

void InintAdc(void)
{
    // Configure P0.27 for AD0.0 function
    // Bits 23:22 of PINSEL1 should be 10 for AD0.0
    PINSEL1 |= (1 << 23);    // Set bit 23
    PINSEL1 &= ~(1 << 22);   // Clear bit 22
    
    AD0CR = 0x00000000;      // Use ADC0, not ADC1
    
    /* Channel 0 selection (AD0.0) */
    AD0CR |= (1 << 0);       // Select channel 0
    
    /* Set clock divider for 4.5 MHz ADC clock (assuming 12MHz PCLK) */
    // CLKDIV = (PCLK / ADC_clk) - 1 = (12MHz / 4.5MHz) - 1 ˜ 1.66 ? use 2
    AD0CR |= (2 << 8);       // CLKDIV = 2
    
    /* Set Burst bit, (Burst = 1) */
    AD0CR |= (1 << 16);
    
    /* Enable ADC 0 */
    AD0CR |= (1 << 21);
}

unsigned int AcquireAdcVal(void)
{
    unsigned int uiAdcVal = 0;
    
    /* Start ADC conversion */
    AD0CR |= (1 << 24);
    
    /* Wait until the end of ADC conversion */
    while(!(END_OF_ADC0_CONVERSION));
    
    /* Read the ADC value */
    uiAdcVal = ((AD0GDR >> 6) & 0x000003FF);
    
    /* Stop the ADC conversion */
    AD0CR &= ~(1 << 24);
    
    /* Return the ADC value */
    return uiAdcVal;
}

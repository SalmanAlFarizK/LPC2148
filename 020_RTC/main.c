#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// LCD Definitions
#define LCD (0xFF << 8)
#define RS  (1 << 16)
#define RW  (1 << 17)
#define EN  (1 << 18)

// RTC Definitions
#define RTC_RST     (1 << 1)
#define RTC_EN      1
#define RTC_32K_SRC (1 << 4)

// Function prototypes
void InitLcd(void);
void Delay(uint16_t uhMs);
void LCD_CMD(char cmd);
void LCD_DAT(unsigned int data);
void LCD_PRINT(char* str);
void rtc_init(void);
void display_time(void);

char time_str[9]; // Buffer for time string "HH:MM:SS"

int main()
{	
    unsigned int prev_sec = 0xFF;
    
    // Configure pins
    PINSEL0 = 0x00000000;  // All P0.0-P0.15 as GPIO (for LCD)
    PINSEL1 = 0x00000000;  // All P1 pins as GPIO
    
    IODIR0 = 0xFFFFFFFF;   // All port 0 as outputs (for LCD)
    IODIR1 = 0x00F00000;   // Keypad rows as outputs
    
    InitLcd();
    rtc_init();
    
    LCD_PRINT("RTC Time:");
    
    while(1)
    {
        if (SEC != prev_sec)
        {
            prev_sec = SEC;
            display_time();
        }
        Delay(100); // Small delay to reduce CPU load
    }
}

void InitLcd(void)
{
    Delay(15);
    
    // Send initialization commands three times (required by some LCD controllers)
    LCD_CMD(0x30);
    Delay(5);
    LCD_CMD(0x30);
    Delay(1);
    LCD_CMD(0x30);
    Delay(1);
    
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
        for(j = 0; j < 6000; ++j);
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
    // Convert number to ASCII character if needed
    if(data < 10) 
    {
        data += '0'; // Convert to ASCII digit
    }
    
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

void rtc_init(void)
{
    // Power up the RTC (if not already done by startup code)
    PCONP |= (1 << 9); // Enable RTC power/clock
    
    // Disable RTC during configuration
    CCR = 0;
    
    // Set PCLK = CCLK/4 (assuming CCLK = 60MHz, PCLK = 15MHz)
    VPBDIV = 0x00;
    
    // Calculate prescaler values for 1Hz clock
    // PREINT = int(PCLK/32768) - 1
    // PREFRAC = PCLK - (PREINT + 1) * 32768
    PREINT = (15000000 / 32768) - 1;    // Should be 457
    PREFRAC = 15000000 - ((PREINT + 1) * 32768); // Should be 25024
    
    // Set initial time
    SEC = 0;
    MIN = 0;
    HOUR = 12;
    DOM = 1;
    DOW = 3; // Wednesday
    DOY = 1;
    MONTH = 1;
    YEAR = 2024;
    
    // Clear interrupt locations
    ILR = 0x03;
    CIIR = 0x00;
    AMR = 0xFF;
    
    // Enable RTC with 32kHz oscillator
    CCR = RTC_EN | RTC_32K_SRC;
}

void display_time(void)
{
    // Move cursor to second line
    LCD_CMD(0xC0);
    
    // Display hours
    LCD_DAT(HOUR / 10 + '0'); // Convert to ASCII
    LCD_DAT(HOUR % 10 + '0'); // Convert to ASCII
    LCD_DAT(':');
    
    // Display minutes
    LCD_DAT(MIN / 10 + '0'); // Convert to ASCII
    LCD_DAT(MIN % 10 + '0'); // Convert to ASCII
    LCD_DAT(':');
    
    // Display seconds
    LCD_DAT(SEC / 10 + '0'); // Convert to ASCII
    LCD_DAT(SEC % 10 + '0'); // Convert to ASCII
}

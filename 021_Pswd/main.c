#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// LCD Definitions
#define LCD (0xFF << 8)
#define RS  (1 << 16)
#define RW  (1 << 17)
#define EN  (1 << 18)

// Keypad Definitions
#define COL1    (1 << 16)
#define COL2    (1 << 17)
#define COL3    (1 << 18)
#define COL4    (1 << 19)
#define ROW1    (1 << 20)
#define ROW2    (1 << 21)
#define ROW3    (1 << 22)
#define ROW4    (1 << 23)

// Password settings
#define PASSWORD_LENGTH 4
const char password[PASSWORD_LENGTH + 1] = "1234"; // Default password

// Function prototypes
void InitLcd(void);
void Delay(uint16_t uhMs);
void LCD_CMD(char cmd);
void LCD_DAT(unsigned int data);
void LCD_PRINT(char* str);
unsigned char KeyPad(void);
void KeyPadDelay(void);
int get_password(char* input_password);
int verify_password(const char* input_password);

int main()
{	
    char input_password[PASSWORD_LENGTH + 1];
    int password_correct = 0;
    
    // Configure pins
    PINSEL0 = 0x00000000;  // All P0.0-P0.15 as GPIO (for LCD)
    PINSEL1 = 0x00000000;  // All P1 pins as GPIO
    
    IODIR0 = 0xFFFFFFFF;   // All port 0 as outputs (for LCD)
    IODIR1 = 0x00F00000;   // Keypad rows as outputs, columns as inputs
    
    InitLcd();
    
    while(1)
    {
        LCD_CMD(0x01);  // Clear display
        LCD_PRINT("Enter Password:");
        LCD_CMD(0xC0);  // Move to second line
        
        // Get password from user
        if(get_password(input_password))
        {
            // Verify password
            password_correct = verify_password(input_password);
            
            LCD_CMD(0x01);  // Clear display
            if(password_correct)
            {
                LCD_PRINT("Access Granted!");
                // Add your access granted code here
                Delay(2000);
            }
            else
            {
                LCD_PRINT("Access Denied!");
                Delay(2000);
            }
        }
    }
}

void InitLcd(void)
{
    Delay(15);
    
    // Send initialization commands three times
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

void KeyPadDelay(void)
{
    unsigned int t1, t2;
    
    for(t1 = 0; t1 < 100; ++t1)
    {
        for(t2 = 0; t2 < 1275; ++t2);
    }
}

unsigned char KeyPad(void)
{
    unsigned char key = 0;
    
    // Check Row 1
    IOCLR1 = ROW1;
    IOSET1 = (ROW2 | ROW3 | ROW4);
    Delay(1);
    
    if(!(IOPIN1 & COL1)) key = '1';
    else if(!(IOPIN1 & COL2)) key = '2';
    else if(!(IOPIN1 & COL3)) key = '3';
    else if(!(IOPIN1 & COL4)) key = 'A';
    
    if(key) 
    {
        KeyPadDelay();
        return key;
    }
    
    // Check Row 2
    IOSET1 = ROW1;
    IOCLR1 = ROW2;
    IOSET1 = (ROW3 | ROW4);
    Delay(1);
    
    if(!(IOPIN1 & COL1)) key = '4';
    else if(!(IOPIN1 & COL2)) key = '5';
    else if(!(IOPIN1 & COL3)) key = '6';
    else if(!(IOPIN1 & COL4)) key = 'B';
    
    if(key) 
    {
        KeyPadDelay();
        return key;
    }
    
    // Check Row 3
    IOSET1 = ROW2;
    IOCLR1 = ROW3;
    IOSET1 = ROW4;
    Delay(1);
    
    if(!(IOPIN1 & COL1)) key = '7';
    else if(!(IOPIN1 & COL2)) key = '8';
    else if(!(IOPIN1 & COL3)) key = '9';
    else if(!(IOPIN1 & COL4)) key = 'C';
    
    if(key) 
    {
        KeyPadDelay();
        return key;
    }
    
    // Check Row 4
    IOSET1 = ROW3;
    IOCLR1 = ROW4;
    Delay(1);
    
    if(!(IOPIN1 & COL1)) key = '*';
    else if(!(IOPIN1 & COL2)) key = '0';
    else if(!(IOPIN1 & COL3)) key = '#';
    else if(!(IOPIN1 & COL4)) key = 'D';
    
    if(key) 
    {
        KeyPadDelay();
        return key;
    }
    
    return 0; // No key pressed
}

int get_password(char* input_password)
{
    int index = 0;
    char key;
    
    // Clear the input buffer
    memset(input_password, 0, PASSWORD_LENGTH + 1);
    
    while(index < PASSWORD_LENGTH)
    {
        key = KeyPad();
        if(key != 0)
        {
            // Only accept numeric keys for password
            if(key >= '0' && key <= '9')
            {
                input_password[index] = key;
                LCD_DAT('*'); // Show asterisk instead of actual digit
                index++;
                Delay(300); // Add delay after each key press
            }
            // If '*' is pressed, clear input
            else if(key == '*')
            {
                LCD_CMD(0xC0); // Move to beginning of second line
                LCD_PRINT("    "); // Clear the input area
                LCD_CMD(0xC0); // Move back
                index = 0;
                memset(input_password, 0, PASSWORD_LENGTH + 1);
                Delay(300);
            }
            // If '#' is pressed, submit password (even if not complete)
            else if(key == '#')
            {
                // Fill remaining characters with null terminator
                while(index < PASSWORD_LENGTH)
                {
                    input_password[index] = '\0';
                    index++;
                }
                return 1; // Password entered
            }
        }
    }
    
    // Auto-submit after entering 4 digits
    return 1;
}

int verify_password(const char* input_password)
{
    return (strcmp(input_password, password) == 0);
}

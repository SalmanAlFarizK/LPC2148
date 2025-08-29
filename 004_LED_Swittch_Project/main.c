#include <lpc214x.h>

/*
* P0.0 ----> LED
* P0.1 ----> Switch
*/

void Delay(int time)
{
    unsigned int i , j;

    for(i = 0; i < time; ++i)
    {
        for(j = 0; j < 7500; ++j)
        {
            // simple delay loop
        }
    }
}

int main(void)
{
    // Configure directions
    IO0DIR |= (1 << 0);    // Set P0.0 as OUTPUT (LED)
    IO0DIR &= ~(1 << 1);   // Set P0.1 as INPUT  (Switch)

    while(1)
    {
        // Check if switch is pressed
        if((IO0PIN & (1 << 1)) == 0)   // Corrected: add parentheses
        {
            IO0SET = (1 << 0);   // Turn LED ON (P0.0 = 1)
        }
        else
        {
            IO0CLR = (1 << 0);   // Turn LED OFF (P0.0 = 0)
        }

        Delay(10);  // small delay to avoid switch bouncing
    }
}

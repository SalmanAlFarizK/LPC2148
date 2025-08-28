#include <lpc214x.h>

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 6000; j++);  // Approx 1ms at 12MHz
    }
}

int main(void)
{
    IO0DIR |= (1 << 0);   // P0.0 as output (LED)
    IO0DIR &= ~(1 << 1);  // P0.1 as input (Switch)

    while(1)
    {
        if((IO0PIN & (1 << 1)) == 0)   // If switch pressed
        {
            IO0CLR = (1 << 0);  // LED OFF
        }
        else
        {
            IO0SET = (1 << 0);  // LED ON
        }
    }
}

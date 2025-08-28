#include <lpc214x.h>

void Delay(int time)
{
	unsigned int i , j;
	
	for(i = 0; i < time; ++i)
	{
		for(j = 0; j < 7500; ++j)
		{
			
		}
	}
}

int main(void)
{
  IO0DIR |= (1 << 0);
  
  while(1)
  {
    IO0SET = (1 << 0);   // Set P0.0 HIGH ? LED ON
    Delay(100);
		IO0CLR = (1 << 0);   // Clear P0.0 LOW ? LED OFF
    Delay(100);
  }
}

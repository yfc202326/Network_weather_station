#include "Delay.h"

void Delay1ms(unsigned int num)		//@22.1184MHz
{
	unsigned char i, j;
	while(num--)
	{
		i = 29;
		j = 183;
		do
		{
			while (--j);
		} while (--i);		
	}
}

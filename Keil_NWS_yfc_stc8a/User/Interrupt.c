#include <STC8A8K64S4A12.H>

void UART1_Routine(void) interrupt 4
{
	if(RI)
	{
		
		RI=0;
	}
}
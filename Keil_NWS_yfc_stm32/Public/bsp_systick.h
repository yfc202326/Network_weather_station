#ifndef __BSP_SYSTICK_H__
#define __BSP_SYSTICK_H__ 

#include "stm32f10x.h"                  // Device header

void SysTick_Delay_us(uint32_t us);
void SysTick_Delay_ms(uint32_t ms);

#endif /* __BSP_SYSTICK_H__ */

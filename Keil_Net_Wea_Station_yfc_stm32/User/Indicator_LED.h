#ifndef __IND_LED_H__
#define __IND_LED_H__ 

#include "stm32f10x.h"                  // Device header

//指示LED端口
#define INDLED_GPIO_CLK			RCC_APB2Periph_GPIOA
#define INDLED_GPIO_PORT		GPIOA
#define INDLED_GPIO_PIN			GPIO_Pin_8

#define	ON						1
#define OFF						0

//LED开关控制
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态
#define Indicator_LED(x)		if(x) \
									GPIO_ResetBits(INDLED_GPIO_PORT,INDLED_GPIO_PIN); \
								else \
									GPIO_SetBits(INDLED_GPIO_PORT,INDLED_GPIO_PIN)
#define Indicator_LED_TOGGLE		 digitalToggle(INDLED_GPIO_PORT,INDLED_GPIO_PIN)
								
void Indicator_LED_GPIO_Init(void);

#endif /* __IND_LED_H__ */								


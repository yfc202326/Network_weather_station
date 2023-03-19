#ifndef _BSP_KEY_H__
#define _BSP_KEY_H__ 

#include "stm32f10x.h"                  // Device header

//引脚定义
#define KEY3_GPIO_CLK		RCC_APB2Periph_GPIOC
#define KEY3_GPIO_PORT		GPIOC
#define KEY3_GPIO_PIN		GPIO_Pin_8

#define KEY4_GPIO_CLK		RCC_APB2Periph_GPIOC
#define KEY4_GPIO_PORT		GPIOC
#define KEY4_GPIO_PIN		GPIO_Pin_9

//按键状态
#define KEY_ON      0
#define KEY_OFF		1

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif /* _BSP_KEY_H__ */


#include "Indicator_LED.h"

//指示等初始化
void Indicator_LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(INDLED_GPIO_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = INDLED_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(INDLED_GPIO_PORT,&GPIO_InitStruct);
	
	Indicator_LED(OFF);
}


#include "bsp_key.h"

/************************************************************************
 @brief  按键GPIO配置
 @param  无
 @retval 无
************************************************************************/
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//打开按键对应GPIO时钟
	RCC_APB2PeriphClockCmd( KEY3_GPIO_CLK|KEY4_GPIO_CLK ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_Init(KEY3_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_Init(KEY3_GPIO_PORT,&GPIO_InitStructure);
}

/************************************************************************
 @brief  按键扫描函数
 @param  按键对应端口
 @retval 按键状态
************************************************************************/
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if( GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )
	{
		while( GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON );
		return KEY_ON;
	}
	else
		return KEY_OFF;
}



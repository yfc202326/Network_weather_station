#include "bsp_key.h"

/************************************************************************
 @brief  ����GPIO����
 @param  ��
 @retval ��
************************************************************************/
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//�򿪰�����ӦGPIOʱ��
	RCC_APB2PeriphClockCmd( KEY3_GPIO_CLK|KEY4_GPIO_CLK ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_Init(KEY3_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_Init(KEY3_GPIO_PORT,&GPIO_InitStructure);
}

/************************************************************************
 @brief  ����ɨ�躯��
 @param  ������Ӧ�˿�
 @retval ����״̬
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



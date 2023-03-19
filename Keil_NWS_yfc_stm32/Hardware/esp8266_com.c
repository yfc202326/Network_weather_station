#include "esp8266_com.h"
 
//Esp8266�ж����ȼ�����
static void Esp8266_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//��λ��ռ����λ������
	
	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//ѡ��USART1
	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ���Ϊ1
	/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ���Ϊ0
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

//Esp8266 GPIO ����
static void Esp8266_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* ��GPIOAʱ�� */
	ESP_USART_GPIO_APBxClkCmd(ESP_USART_GPIO_CLK,ENABLE);
	
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = ESP_USART_TX_GPIO_PIN;//ѡ��PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(ESP_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = ESP_USART_RX_GPIO_PIN;//ѡ��PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//�������루���벻������Ƶ�ʣ�
	GPIO_Init(ESP_USART_RX_GPIO_PORT, &GPIO_InitStructure);	
}

//Esp8266 ģʽ����
static void Esp8266_Mode_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	// �򿪴��������ʱ��
	ESP_USART_APBxClkCmd(ESP_USART_CLK, ENABLE);//��USART1ʱ��
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = ESP_USART_BAUDRATE;//115200bps
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ��Чλ
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;//��У��
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���պͷ���
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(ESP_USARTx, &USART_InitStructure);
	
	
	// ʹ�ܴ��ڽ����жϣ�����ʹ�ܷ����жϣ�ע�⣺ʹ��scanf()��Ҫ������䣬��Ϊ����Ҫ�����ж�
	/* RXNEIE bit of USART_CR1 register */
	//USART_ITConfig(ESP_USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(ESP_USARTx, ENABLE);//ʹ��UE(����)
}

//Esp8266��ʼ��
void Esp8266_Init(void)
{
	Esp8266_NVIC_Config();
	Esp8266_GPIO_Config();
	Esp8266_Mode_Config();
}

/*************************************************************************
 @brief  ���ڷ���1���ֽ�����
 @param  pUSARTx����Ӧ�Ĵ��ںţ�x:1~5
		 Data��Ҫ���͵�����
 @retval None.
*************************************************************************/
void Esp8266_SendByte(USART_TypeDef *pUSARTx, uint8_t Byte)
{
	USART_SendData(pUSARTx,Byte);
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET );
}

/*************************************************************************
 @brief  ���ڷ���2���ֽ�����
 @param  pUSARTx����Ӧ�Ĵ��ںţ�x:1~5
		 Data��Ҫ���͵�����
 @retval None.
*************************************************************************/
void Esp8266_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t HalfWord)
{
	uint8_t temp_h,temp_l;
	
	temp_h = (HalfWord & 0xFF00)>>8;
	temp_l = (HalfWord & 0xFF);
	
	USART_SendData(pUSARTx,temp_h);
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET );
	USART_SendData(pUSARTx,temp_l);
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET );
}

/*************************************************************************
 @brief  ���ڷ�������
 @param  pUSARTx:��Ӧ�Ĵ��ںţ�x:1~5
		 array:Ҫ���͵�������
		 num:����Ĵ�С
 @retval None.
*************************************************************************/
void Esp8266_SendArray(USART_TypeDef *pUSARTx, uint8_t *array,uint8_t num)
{
	uint8_t i;
	for(i=0; i<num; i++)
		Esp8266_SendByte(pUSARTx,array[i]);
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET );
}

/*************************************************************************
 @brief  ���ڷ����ַ���
 @param  pUSARTx:��Ӧ�Ĵ��ںţ�x:1~5
		 str:Ҫ���͵��ַ���
 @retval None.
*************************************************************************/
void Esp8266_SendString(USART_TypeDef *pUSARTx, char *str)//ע��char,signed char,unsigned char����
{
	while(*str!='\0')
	{
		Esp8266_SendByte(pUSARTx, *str++);
	}
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET );
}

//Esp8266��������
void Esp8266_SendCmd(char *cmd)
{
	uint8_t buffer;
	uint8_t state=0;
	
	Esp8266_SendString(ESP_USARTx,cmd);

	//�ȴ�����OK
	while(1)
	{		
		if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE)==SET)
		{
			buffer=USART_ReceiveData(ESP_USARTx);
			USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
			Usart_SendByte(DEBUG_USARTx,buffer);
			if(buffer=='O')
			{
				state=1;
			}
			else if(state==1 && buffer=='K')
			{
				state=0;
				Usart_SendString(DEBUG_USARTx,"\r\n");
				break;
			}
		}
	}	
}



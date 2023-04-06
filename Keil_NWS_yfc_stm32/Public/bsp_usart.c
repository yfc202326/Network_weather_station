#include "bsp_usart.h"

/*************************************************************************
 @brief  ����Ƕ�������жϿ�����NVIC
 @param  None.
 @retval None.
*************************************************************************/
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//��λ��ռ����λ������
  
	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;//ѡ��USART1
	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ���Ϊ1
	/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ���Ϊ1
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

/*************************************************************************
 @brief  USART GPIO ����,������������
 @param  None.
 @retval None.
*************************************************************************/
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);//��GPIOAʱ��
	
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);//��USART1ʱ��

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;//ѡ��PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;//ѡ��PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//�������루���벻������Ƶ�ʣ�
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;//115200bps
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
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����жϣ�����ʹ�ܷ����жϣ�ע�⣺ʹ��scanf()��Ҫ������䣬��Ϊ����Ҫ�����ж�
//	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);//ʹ��UE(����)
}

/*************************************************************************
 @brief  ���ڷ���1���ֽ�����
 @param  pUSARTx����Ӧ�Ĵ��ںţ�x:1~5
		 Data��Ҫ���͵�����
 @retval None.
*************************************************************************/
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t Data)
{
	USART_SendData(pUSARTx,Data);
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET );
}

/*************************************************************************
 @brief  ���ڷ���2���ֽ�����
 @param  pUSARTx����Ӧ�Ĵ��ںţ�x:1~5
		 Data��Ҫ���͵�����
 @retval None.
*************************************************************************/
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t Data)
{
	uint8_t temp_h,temp_l;
	
	temp_h = (Data & 0xFF00)>>8;
	temp_l = (Data & 0xFF);
	
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
void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array,uint8_t num)
{
	uint8_t i;
	for(i=0; i<num; i++)
		Usart_SendByte(pUSARTx,array[i]);
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET );
}

/*************************************************************************
 @brief  ���ڷ����ַ���
 @param  pUSARTx:��Ӧ�Ĵ��ںţ�x:1~5
		 str:Ҫ���͵��ַ���
 @retval None.
*************************************************************************/
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)//ע��char,signed char,unsigned char����
{
	while(*str!='\0')
	{
		Usart_SendByte(pUSARTx, *str++);
	}
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET );
}

/*************************************************************************
 @brief  �ض���C�⺯��printf()�����ڣ��ض�����ʹ��printf()
 @param  ch:Ҫ���͵�����
		 f:�ļ�ָ��
 @retval ch.
*************************************************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(DEBUG_USARTx,(uint8_t)(ch));
	while( USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_TXE) == RESET );
	return (ch);
}

/*************************************************************************
 @brief  �ض���C�⺯��scanf()�����ڣ��ض�����ʹ��scanf()
 @param  f:�ļ�ָ��
 @retval USART_ReceiveData(USART1)
*************************************************************************/
int fgetc(FILE *f)
{
	while( USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_RXNE) == RESET );
	return (int)(USART_ReceiveData(DEBUG_USARTx));
}


#include "bsp_usart.h"

/*************************************************************************
 @brief  配置嵌套向量中断控制器NVIC
 @param  None.
 @retval None.
*************************************************************************/
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//两位抢占，两位次优先
  
	/* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;//选择USART1
	/* 抢断优先级*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级设为1
	/* 子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//子优先级设为1
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能
	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

/*************************************************************************
 @brief  USART GPIO 配置,工作参数配置
 @param  None.
 @retval None.
*************************************************************************/
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);//打开GPIOA时钟
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);//打开USART1时钟

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;//选择PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//频率50MHz
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;//选择PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入（输入不需设置频率）
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;//115200bps
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位有效位
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;//无校验
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不启用
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收和发送
	
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断（不用使能发送中断）注意：使用scanf()不要以下语句，因为不需要接收中断
//	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);//使能UE(大门)
}

/*************************************************************************
 @brief  串口发送1个字节数据
 @param  pUSARTx：对应的串口号，x:1~5
		 Data：要发送的数据
 @retval None.
*************************************************************************/
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t Data)
{
	USART_SendData(pUSARTx,Data);
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET );
}

/*************************************************************************
 @brief  串口发送2个字节数据
 @param  pUSARTx：对应的串口号，x:1~5
		 Data：要发送的数据
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
 @brief  串口发送数组
 @param  pUSARTx:对应的串口号，x:1~5
		 array:要发送的数组名
		 num:数组的大小
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
 @brief  串口发送字符串
 @param  pUSARTx:对应的串口号，x:1~5
		 str:要发送的字符串
 @retval None.
*************************************************************************/
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)//注意char,signed char,unsigned char区别
{
	while(*str!='\0')
	{
		Usart_SendByte(pUSARTx, *str++);
	}
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET );
}

/*************************************************************************
 @brief  重定义C库函数printf()到串口，重定义后可使用printf()
 @param  ch:要发送的数据
		 f:文件指针
 @retval ch.
*************************************************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(DEBUG_USARTx,(uint8_t)(ch));
	while( USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_TXE) == RESET );
	return (ch);
}

/*************************************************************************
 @brief  重定义C库函数scanf()到串口，重定义后可使用scanf()
 @param  f:文件指针
 @retval USART_ReceiveData(USART1)
*************************************************************************/
int fgetc(FILE *f)
{
	while( USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_RXNE) == RESET );
	return (int)(USART_ReceiveData(DEBUG_USARTx));
}


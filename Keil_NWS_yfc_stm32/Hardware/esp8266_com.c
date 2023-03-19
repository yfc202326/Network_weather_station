#include "esp8266_com.h"
 
//Esp8266中断优先级配置
static void Esp8266_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//两位抢占，两位次优先
	
	/* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//选择USART1
	/* 抢断优先级*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级设为1
	/* 子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//子优先级设为0
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能
	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

//Esp8266 GPIO 配置
static void Esp8266_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* 打开GPIOA时钟 */
	ESP_USART_GPIO_APBxClkCmd(ESP_USART_GPIO_CLK,ENABLE);
	
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = ESP_USART_TX_GPIO_PIN;//选择PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//频率50MHz
	GPIO_Init(ESP_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	// 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = ESP_USART_RX_GPIO_PIN;//选择PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入（输入不需设置频率）
	GPIO_Init(ESP_USART_RX_GPIO_PORT, &GPIO_InitStructure);	
}

//Esp8266 模式配置
static void Esp8266_Mode_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	// 打开串口外设的时钟
	ESP_USART_APBxClkCmd(ESP_USART_CLK, ENABLE);//打开USART1时钟
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = ESP_USART_BAUDRATE;//115200bps
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
	USART_Init(ESP_USARTx, &USART_InitStructure);
	
	
	// 使能串口接收中断（不用使能发送中断）注意：使用scanf()不要以下语句，因为不需要接收中断
	/* RXNEIE bit of USART_CR1 register */
	//USART_ITConfig(ESP_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(ESP_USARTx, ENABLE);//使能UE(大门)
}

//Esp8266初始化
void Esp8266_Init(void)
{
	Esp8266_NVIC_Config();
	Esp8266_GPIO_Config();
	Esp8266_Mode_Config();
}

/*************************************************************************
 @brief  串口发送1个字节数据
 @param  pUSARTx：对应的串口号，x:1~5
		 Data：要发送的数据
 @retval None.
*************************************************************************/
void Esp8266_SendByte(USART_TypeDef *pUSARTx, uint8_t Byte)
{
	USART_SendData(pUSARTx,Byte);
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET );
}

/*************************************************************************
 @brief  串口发送2个字节数据
 @param  pUSARTx：对应的串口号，x:1~5
		 Data：要发送的数据
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
 @brief  串口发送数组
 @param  pUSARTx:对应的串口号，x:1~5
		 array:要发送的数组名
		 num:数组的大小
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
 @brief  串口发送字符串
 @param  pUSARTx:对应的串口号，x:1~5
		 str:要发送的字符串
 @retval None.
*************************************************************************/
void Esp8266_SendString(USART_TypeDef *pUSARTx, char *str)//注意char,signed char,unsigned char区别
{
	while(*str!='\0')
	{
		Esp8266_SendByte(pUSARTx, *str++);
	}
	while( USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET );
}

//Esp8266发送命令
void Esp8266_SendCmd(char *cmd)
{
	uint8_t buffer;
	uint8_t state=0;
	
	Esp8266_SendString(ESP_USARTx,cmd);

	//等待接收OK
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



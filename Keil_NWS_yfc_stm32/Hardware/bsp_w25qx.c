#include "bsp_w25qx.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

/**
  * @brief  w25qx SPI-GPIO 配置
  * @param  无
  * @retval 无
  */
static void W25qx_SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 使能SPI引脚相关的时钟 */
 	W25QX_SPI_CS_APBxClock_FUN ( W25QX_SPI_CS_CLK|W25QX_SPI_SCK_CLK|W25QX_SPI_MISO_PIN|W25QX_SPI_MOSI_PIN, ENABLE );
	
	/* 配置SPI的 CS引脚，普通IO即可 */
	GPIO_InitStructure.GPIO_Pin = W25QX_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(W25QX_SPI_CS_PORT, &GPIO_InitStructure);
	
	/* 配置SPI的 SCK引脚*/
	GPIO_InitStructure.GPIO_Pin = W25QX_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(W25QX_SPI_SCK_PORT, &GPIO_InitStructure);

	/* 配置SPI的 MISO引脚*/
	GPIO_InitStructure.GPIO_Pin = W25QX_SPI_MISO_PIN;
	GPIO_Init(W25QX_SPI_MISO_PORT, &GPIO_InitStructure);

	/* 配置SPI的 MOSI引脚*/
	GPIO_InitStructure.GPIO_Pin = W25QX_SPI_MOSI_PIN;
	GPIO_Init(W25QX_SPI_MOSI_PORT, &GPIO_InitStructure);
	
	/* 停止信号 FLASH: CS引脚高电平*/
	W25QX_SPI_CS_HIGH();
}

/**
  * @brief  w25qx SPI-模式 配置
  * @param  无
  * @retval 无
  */
static void W25qx_SPI_MODE_Config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	/* 使能SPI时钟 */
	W25QX_SPI_APBxClock_FUN ( W25QX_SPI_CLK, ENABLE );
	
	/* SPI 模式配置 */
	// FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8bits
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//时钟极性为高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//时钟相位为偶
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//软件控制片选
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//时钟4分频
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高位优先传输
	SPI_InitStructure.SPI_CRCPolynomial = 7;//此处不用CRC，随便一个数字就行
  
	SPI_Init(W25QX_SPIx , &SPI_InitStructure);
	/* 使能 SPI  */
	SPI_Cmd(W25QX_SPIx , ENABLE);	
}

/**
  * @brief  w25qx 初始化
  * @param  无
  * @retval 无
  */
void W25qx_Init(void)
{
	W25qx_SPI_GPIO_Config();
	W25qx_SPI_MODE_Config();
}


//以上为初始化代码
/*-------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------*/
//以下为具体操作函数


/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  W25QX_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}

 /**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
u8 W25qx_SPI_SendByte(u8 byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区为空，TXE事件 */
	while (SPI_I2S_GetFlagStatus(W25QX_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}

	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_I2S_SendData(W25QX_SPIx , byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待接收缓冲区非空，RXNE事件 */
	while (SPI_I2S_GetFlagStatus(W25QX_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
	}

	/* 读取数据寄存器，获取接收缓冲区数据 */
	return SPI_I2S_ReceiveData(W25QX_SPIx );
}

 /**
  * @brief  使用SPI发送两个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
u16 W25qx_SPI_SendHalfWord(u16 HalfWord)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区为空，TXE事件 */
	while (SPI_I2S_GetFlagStatus(W25QX_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
	}
	
	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_I2S_SendData(W25QX_SPIx , HalfWord);

	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待接收缓冲区非空，RXNE事件 */
	while (SPI_I2S_GetFlagStatus(W25QX_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
	}
	/* 读取数据寄存器，获取接收缓冲区数据 */
	return SPI_I2S_ReceiveData(W25QX_SPIx );
}

 /**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
u8 W25qx_SPI_ReadByte(void)
{
	return (W25qx_SPI_SendByte(Dummy_Byte));
}

 /**
  * @brief  向FLASH发送 写使能 命令
  * @param  none
  * @retval none
  */
void W25qx_SPI_WriteEnable(void)
{
	/* 通讯开始：CS低 */
	W25QX_SPI_CS_LOW();

	/* 发送写使能命令*/
	W25qx_SPI_SendByte(W25X_WriteEnable);

	/*通讯结束：CS高 */
	W25QX_SPI_CS_HIGH();
}

 /**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void W25qx_SPI_WaitForWriteEnd(void)
{
	u8 FLASH_Status = 0;

	/* 选择 FLASH: CS 低 */
	W25QX_SPI_CS_LOW();

	/* 发送 读状态寄存器 命令 */
	W25qx_SPI_SendByte(W25X_ReadStatusReg);

	/* 若FLASH忙碌，则等待 */
	do
	{
		/* 读取FLASH芯片的状态寄存器 */
		FLASH_Status = W25qx_SPI_SendByte(Dummy_Byte);	 
	}
	while ((FLASH_Status & WIP_Flag) == SET);  /* 正在写入标志 */

	/* 停止信号  FLASH: CS 高 */
	W25QX_SPI_CS_HIGH();
}

 /**
  * @brief  进入掉电模式
  * @param  none
  * @retval none
  */
void W25qx_SPI_PowerDown(void)   
{ 
	/* 通讯开始：CS低 */
	W25QX_SPI_CS_LOW();

	/* 发送 掉电 命令 */
	W25qx_SPI_SendByte(W25X_PowerDown);

	/*通讯结束：CS高 */
	W25QX_SPI_CS_HIGH();
}   

 /**
  * @brief  唤醒
  * @param  none
  * @retval none
  */
void W25qx_SPI_WAKEUP(void)   
{
	/*选择 FLASH: CS 低 */
	W25QX_SPI_CS_LOW();

	/* 发送 上电 命令 */
	W25qx_SPI_SendByte(W25X_ReleasePowerDown);

	/* 停止信号 FLASH: CS 高 */
	W25QX_SPI_CS_HIGH();
} 

 /**
  * @brief  擦除FLASH扇区
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void W25qx_SPI_SectorErase(u32 SectorAddr)
{
	/* 发送FLASH写使能命令 */
	W25qx_SPI_WriteEnable();
	W25qx_SPI_WaitForWriteEnd();
	/* 擦除扇区 */
	/* 选择FLASH: CS低电平 */
	W25QX_SPI_CS_LOW();
	/* 发送扇区擦除指令*/
	W25qx_SPI_SendByte(W25X_SectorErase);
	/*发送擦除扇区地址的高位*/
	W25qx_SPI_SendByte((SectorAddr & 0xFF0000) >> 16);
	/* 发送擦除扇区地址的中位 */
	W25qx_SPI_SendByte((SectorAddr & 0xFF00) >> 8);
	/* 发送擦除扇区地址的低位 */
	W25qx_SPI_SendByte(SectorAddr & 0xFF);
	/* 停止信号 FLASH: CS 高电平 */
	W25QX_SPI_CS_HIGH();
	/* 等待擦除完毕*/
	W25qx_SPI_WaitForWriteEnd();
}

 /**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无
  * @retval 无
  */
void W25qx_SPI_BulkErase(void)
{
	/* 发送FLASH写使能命令 */
	W25qx_SPI_WriteEnable();

	/* 整块 Erase */
	/* 选择FLASH: CS低电平 */
	W25QX_SPI_CS_LOW();
	/* 发送整块擦除指令*/
	W25qx_SPI_SendByte(W25X_ChipErase);
	/* 停止信号 FLASH: CS 高电平 */
	W25QX_SPI_CS_HIGH();

	/* 等待擦除完毕*/
	W25qx_SPI_WaitForWriteEnd();
}

 /**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * @retval 无
  */
void W25qx_SPI_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	/* 发送FLASH写使能命令 */
	W25qx_SPI_WriteEnable();

	/* 选择FLASH: CS低电平 */
	W25QX_SPI_CS_LOW();
	/* 写页写指令*/
	W25qx_SPI_SendByte(W25X_PageProgram);
	/*发送写地址的高位*/
	W25qx_SPI_SendByte((WriteAddr & 0xFF0000) >> 16);
	/*发送写地址的中位*/
	W25qx_SPI_SendByte((WriteAddr & 0xFF00) >> 8);
	/*发送写地址的低位*/
	W25qx_SPI_SendByte(WriteAddr & 0xFF);

	if(NumByteToWrite > W25QX_SPI_PerWritePageSize)
	{
		NumByteToWrite = W25QX_SPI_PerWritePageSize;
		W25QX_ERROR("SPI_FLASH_PageWrite too large!"); 
	}

	/* 写入数据*/
	while (NumByteToWrite--)
	{
		/* 发送当前要写入的字节数据 */
		W25qx_SPI_SendByte(*pBuffer);
		/* 指向下一字节数据 */
		pBuffer++;
	}

	/* 停止信号 FLASH: CS 高电平 */
	W25QX_SPI_CS_HIGH();

	/* 等待写入完毕*/
	W25qx_SPI_WaitForWriteEnd();
}

/**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval 无
  */
void W25qx_SPI_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
	Addr = WriteAddr % W25QX_SPI_PageSize;
	
	/*差count个数据值，刚好可以对齐到页地址*/
	count = W25QX_SPI_PageSize - Addr;
	/*计算出要写多少整数页*/
	NumOfPage =  NumByteToWrite / W25QX_SPI_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
	NumOfSingle = NumByteToWrite % W25QX_SPI_PageSize;
	
	/* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
	if (Addr == 0)
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
		if (NumOfPage == 0) 
		{
			W25qx_SPI_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{ 
			/*先把整数页都写了*/
			while (NumOfPage--)
			{
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, W25QX_SPI_PageSize);
				WriteAddr +=  W25QX_SPI_PageSize;
				pBuffer += W25QX_SPI_PageSize;
		}
		/*若有多余的不满一页的数据，把它写完*/
		W25qx_SPI_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
	else 
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
		if (NumOfPage == 0)
		{
			/*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
			if (NumOfSingle > count) 
			{
				temp = NumOfSingle - count;
				/*先写满当前页*/
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, count);
				
				WriteAddr +=  count;
				pBuffer += count;
				/*再写剩余的数据*/
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, temp);
			}
			else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
			{
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{
			/*地址不对齐多出的count分开处理，不加入这个运算*/
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / W25QX_SPI_PageSize;
			NumOfSingle = NumByteToWrite % W25QX_SPI_PageSize;
			
			/* 先写完count个数据，为的是让下一次要写的地址对齐 */
			W25qx_SPI_PageWrite(pBuffer, WriteAddr, count);
			
			/* 接下来就重复地址对齐的情况 */
			WriteAddr +=  count;
			pBuffer += count;
			/*把整数页都写了*/
			while (NumOfPage--)
			{
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, W25QX_SPI_PageSize);
				WriteAddr +=  W25QX_SPI_PageSize;
				pBuffer += W25QX_SPI_PageSize;
			}
			/*若有多余的不满一页的数据，把它写完*/
			if (NumOfSingle != 0)
			{
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}

 /**
  * @brief  读取FLASH数据
  * @param 	pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度(Byte)
  * @retval 无
  */
void W25qx_SPI_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	/* 选择FLASH: CS低电平 */
	W25QX_SPI_CS_LOW();

	/* 发送 读 指令 */
	W25qx_SPI_SendByte(W25X_ReadData);

	/* 发送 读 地址高位 */
	W25qx_SPI_SendByte((ReadAddr & 0xFF0000) >> 16);
	/* 发送 读 地址中位 */
	W25qx_SPI_SendByte((ReadAddr& 0xFF00) >> 8);
	/* 发送 读 地址低位 */
	W25qx_SPI_SendByte(ReadAddr & 0xFF);
	
	/* 读取数据 */
	while (NumByteToRead--) /* while there is data to be read */
	{
		/* 读取一个字节*/
		*pBuffer = W25qx_SPI_SendByte(Dummy_Byte);
		/* 指向下一个字节缓冲区 */
		pBuffer++;
	}

	/* 停止信号 FLASH: CS 高电平 */
	W25QX_SPI_CS_HIGH();
}

 /**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
u32 W25qx_SPI_ReadID(void)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/* 开始通讯：CS低电平 */
	W25QX_SPI_CS_LOW();

	/* 发送JEDEC指令，读取ID */
	W25qx_SPI_SendByte(W25X_JedecDeviceID);

	/* 读取一个字节数据 */
	Temp0 = W25qx_SPI_SendByte(Dummy_Byte);

	/* 读取一个字节数据 */
	Temp1 = W25qx_SPI_SendByte(Dummy_Byte);

	/* 读取一个字节数据 */
	Temp2 = W25qx_SPI_SendByte(Dummy_Byte);

	/* 停止通讯：CS高电平 */
	W25QX_SPI_CS_HIGH();

	/*把数据组合起来，作为函数的返回值*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}

 /**
  * @brief  读取FLASH Device ID
  * @param 	无
  * @retval FLASH Device ID
  */
u32 W25qx_SPI_ReadDeviceID(void)
{
	u32 Temp = 0;

	/* Select the FLASH: Chip Select low */
	W25QX_SPI_CS_LOW();

	/* Send "RDID " instruction */
	W25qx_SPI_SendByte(W25X_DeviceID);
	W25qx_SPI_SendByte(Dummy_Byte);
	W25qx_SPI_SendByte(Dummy_Byte);
	W25qx_SPI_SendByte(Dummy_Byte);
  
	/* Read a byte from the FLASH */
	Temp = W25qx_SPI_SendByte(Dummy_Byte);

	/* Deselect the FLASH: Chip Select high */
	W25QX_SPI_CS_HIGH();

	return Temp;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void W25qx_SPI_StartReadSequence(u32 ReadAddr)
{
	/* Select the FLASH: Chip Select low */
	W25QX_SPI_CS_LOW();

	/* Send "Read from Memory " instruction */
	W25qx_SPI_SendByte(W25X_ReadData);

	/* Send the 24-bit address of the address to read from -----------------------*/
	/* Send ReadAddr high nibble address byte */
	W25qx_SPI_SendByte((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte */
	W25qx_SPI_SendByte((ReadAddr& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte */
	W25qx_SPI_SendByte(ReadAddr & 0xFF);
}


/*********************************************END OF FILE**********************/




#include "bsp_w25qx.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

/**
  * @brief  w25qx SPI-GPIO ����
  * @param  ��
  * @retval ��
  */
static void W25qx_SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ��SPI������ص�ʱ�� */
 	W25QX_SPI_CS_APBxClock_FUN ( W25QX_SPI_CS_CLK|W25QX_SPI_SCK_CLK|W25QX_SPI_MISO_PIN|W25QX_SPI_MOSI_PIN, ENABLE );
	
	/* ����SPI�� CS���ţ���ͨIO���� */
	GPIO_InitStructure.GPIO_Pin = W25QX_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(W25QX_SPI_CS_PORT, &GPIO_InitStructure);
	
	/* ����SPI�� SCK����*/
	GPIO_InitStructure.GPIO_Pin = W25QX_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(W25QX_SPI_SCK_PORT, &GPIO_InitStructure);

	/* ����SPI�� MISO����*/
	GPIO_InitStructure.GPIO_Pin = W25QX_SPI_MISO_PIN;
	GPIO_Init(W25QX_SPI_MISO_PORT, &GPIO_InitStructure);

	/* ����SPI�� MOSI����*/
	GPIO_InitStructure.GPIO_Pin = W25QX_SPI_MOSI_PIN;
	GPIO_Init(W25QX_SPI_MOSI_PORT, &GPIO_InitStructure);
	
	/* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
	W25QX_SPI_CS_HIGH();
}

/**
  * @brief  w25qx SPI-ģʽ ����
  * @param  ��
  * @retval ��
  */
static void W25qx_SPI_MODE_Config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	/* ʹ��SPIʱ�� */
	W25QX_SPI_APBxClock_FUN ( W25QX_SPI_CLK, ENABLE );
	
	/* SPI ģʽ���� */
	// FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8bits
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//ʱ�Ӽ���Ϊ��
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//ʱ����λΪż
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//�������Ƭѡ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//ʱ��4��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//��λ���ȴ���
	SPI_InitStructure.SPI_CRCPolynomial = 7;//�˴�����CRC�����һ�����־���
  
	SPI_Init(W25QX_SPIx , &SPI_InitStructure);
	/* ʹ�� SPI  */
	SPI_Cmd(W25QX_SPIx , ENABLE);	
}

/**
  * @brief  w25qx ��ʼ��
  * @param  ��
  * @retval ��
  */
void W25qx_Init(void)
{
	W25qx_SPI_GPIO_Config();
	W25qx_SPI_MODE_Config();
}


//����Ϊ��ʼ������
/*-------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------*/
//����Ϊ�����������


/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  W25QX_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}

 /**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u8 W25qx_SPI_SendByte(u8 byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
	while (SPI_I2S_GetFlagStatus(W25QX_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}

	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	SPI_I2S_SendData(W25QX_SPIx , byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* �ȴ����ջ������ǿգ�RXNE�¼� */
	while (SPI_I2S_GetFlagStatus(W25QX_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
	}

	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return SPI_I2S_ReceiveData(W25QX_SPIx );
}

 /**
  * @brief  ʹ��SPI���������ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u16 W25qx_SPI_SendHalfWord(u16 HalfWord)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
	while (SPI_I2S_GetFlagStatus(W25QX_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
	}
	
	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	SPI_I2S_SendData(W25QX_SPIx , HalfWord);

	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* �ȴ����ջ������ǿգ�RXNE�¼� */
	while (SPI_I2S_GetFlagStatus(W25QX_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
	}
	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return SPI_I2S_ReceiveData(W25QX_SPIx );
}

 /**
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  ��
  * @retval ���ؽ��յ�������
  */
u8 W25qx_SPI_ReadByte(void)
{
	return (W25qx_SPI_SendByte(Dummy_Byte));
}

 /**
  * @brief  ��FLASH���� дʹ�� ����
  * @param  none
  * @retval none
  */
void W25qx_SPI_WriteEnable(void)
{
	/* ͨѶ��ʼ��CS�� */
	W25QX_SPI_CS_LOW();

	/* ����дʹ������*/
	W25qx_SPI_SendByte(W25X_WriteEnable);

	/*ͨѶ������CS�� */
	W25QX_SPI_CS_HIGH();
}

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void W25qx_SPI_WaitForWriteEnd(void)
{
	u8 FLASH_Status = 0;

	/* ѡ�� FLASH: CS �� */
	W25QX_SPI_CS_LOW();

	/* ���� ��״̬�Ĵ��� ���� */
	W25qx_SPI_SendByte(W25X_ReadStatusReg);

	/* ��FLASHæµ����ȴ� */
	do
	{
		/* ��ȡFLASHоƬ��״̬�Ĵ��� */
		FLASH_Status = W25qx_SPI_SendByte(Dummy_Byte);	 
	}
	while ((FLASH_Status & WIP_Flag) == SET);  /* ����д���־ */

	/* ֹͣ�ź�  FLASH: CS �� */
	W25QX_SPI_CS_HIGH();
}

 /**
  * @brief  �������ģʽ
  * @param  none
  * @retval none
  */
void W25qx_SPI_PowerDown(void)   
{ 
	/* ͨѶ��ʼ��CS�� */
	W25QX_SPI_CS_LOW();

	/* ���� ���� ���� */
	W25qx_SPI_SendByte(W25X_PowerDown);

	/*ͨѶ������CS�� */
	W25QX_SPI_CS_HIGH();
}   

 /**
  * @brief  ����
  * @param  none
  * @retval none
  */
void W25qx_SPI_WAKEUP(void)   
{
	/*ѡ�� FLASH: CS �� */
	W25QX_SPI_CS_LOW();

	/* ���� �ϵ� ���� */
	W25qx_SPI_SendByte(W25X_ReleasePowerDown);

	/* ֹͣ�ź� FLASH: CS �� */
	W25QX_SPI_CS_HIGH();
} 

 /**
  * @brief  ����FLASH����
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void W25qx_SPI_SectorErase(u32 SectorAddr)
{
	/* ����FLASHдʹ������ */
	W25qx_SPI_WriteEnable();
	W25qx_SPI_WaitForWriteEnd();
	/* �������� */
	/* ѡ��FLASH: CS�͵�ƽ */
	W25QX_SPI_CS_LOW();
	/* ������������ָ��*/
	W25qx_SPI_SendByte(W25X_SectorErase);
	/*���Ͳ���������ַ�ĸ�λ*/
	W25qx_SPI_SendByte((SectorAddr & 0xFF0000) >> 16);
	/* ���Ͳ���������ַ����λ */
	W25qx_SPI_SendByte((SectorAddr & 0xFF00) >> 8);
	/* ���Ͳ���������ַ�ĵ�λ */
	W25qx_SPI_SendByte(SectorAddr & 0xFF);
	/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
	W25QX_SPI_CS_HIGH();
	/* �ȴ��������*/
	W25qx_SPI_WaitForWriteEnd();
}

 /**
  * @brief  ����FLASH��������Ƭ����
  * @param  ��
  * @retval ��
  */
void W25qx_SPI_BulkErase(void)
{
	/* ����FLASHдʹ������ */
	W25qx_SPI_WriteEnable();

	/* ���� Erase */
	/* ѡ��FLASH: CS�͵�ƽ */
	W25QX_SPI_CS_LOW();
	/* �����������ָ��*/
	W25qx_SPI_SendByte(W25X_ChipErase);
	/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
	W25QX_SPI_CS_HIGH();

	/* �ȴ��������*/
	W25qx_SPI_WaitForWriteEnd();
}

 /**
  * @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
  * @retval ��
  */
void W25qx_SPI_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	/* ����FLASHдʹ������ */
	W25qx_SPI_WriteEnable();

	/* ѡ��FLASH: CS�͵�ƽ */
	W25QX_SPI_CS_LOW();
	/* дҳдָ��*/
	W25qx_SPI_SendByte(W25X_PageProgram);
	/*����д��ַ�ĸ�λ*/
	W25qx_SPI_SendByte((WriteAddr & 0xFF0000) >> 16);
	/*����д��ַ����λ*/
	W25qx_SPI_SendByte((WriteAddr & 0xFF00) >> 8);
	/*����д��ַ�ĵ�λ*/
	W25qx_SPI_SendByte(WriteAddr & 0xFF);

	if(NumByteToWrite > W25QX_SPI_PerWritePageSize)
	{
		NumByteToWrite = W25QX_SPI_PerWritePageSize;
		W25QX_ERROR("SPI_FLASH_PageWrite too large!"); 
	}

	/* д������*/
	while (NumByteToWrite--)
	{
		/* ���͵�ǰҪд����ֽ����� */
		W25qx_SPI_SendByte(*pBuffer);
		/* ָ����һ�ֽ����� */
		pBuffer++;
	}

	/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
	W25QX_SPI_CS_HIGH();

	/* �ȴ�д�����*/
	W25qx_SPI_WaitForWriteEnd();
}

/**
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  * @retval ��
  */
void W25qx_SPI_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
	Addr = WriteAddr % W25QX_SPI_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
	count = W25QX_SPI_PageSize - Addr;
	/*�����Ҫд��������ҳ*/
	NumOfPage =  NumByteToWrite / W25QX_SPI_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
	NumOfSingle = NumByteToWrite % W25QX_SPI_PageSize;
	
	/* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
	if (Addr == 0)
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
		if (NumOfPage == 0) 
		{
			W25qx_SPI_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{ 
			/*�Ȱ�����ҳ��д��*/
			while (NumOfPage--)
			{
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, W25QX_SPI_PageSize);
				WriteAddr +=  W25QX_SPI_PageSize;
				pBuffer += W25QX_SPI_PageSize;
		}
		/*���ж���Ĳ���һҳ�����ݣ�����д��*/
		W25qx_SPI_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	/* ����ַ�� SPI_FLASH_PageSize ������  */
	else 
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
		if (NumOfPage == 0)
		{
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����*/
			if (NumOfSingle > count) 
			{
				temp = NumOfSingle - count;
				/*��д����ǰҳ*/
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, count);
				
				WriteAddr +=  count;
				pBuffer += count;
				/*��дʣ�������*/
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, temp);
			}
			else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
			{
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{
			/*��ַ����������count�ֿ������������������*/
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / W25QX_SPI_PageSize;
			NumOfSingle = NumByteToWrite % W25QX_SPI_PageSize;
			
			/* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
			W25qx_SPI_PageWrite(pBuffer, WriteAddr, count);
			
			/* ���������ظ���ַ�������� */
			WriteAddr +=  count;
			pBuffer += count;
			/*������ҳ��д��*/
			while (NumOfPage--)
			{
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, W25QX_SPI_PageSize);
				WriteAddr +=  W25QX_SPI_PageSize;
				pBuffer += W25QX_SPI_PageSize;
			}
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
			if (NumOfSingle != 0)
			{
				W25qx_SPI_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}

 /**
  * @brief  ��ȡFLASH����
  * @param 	pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���(Byte)
  * @retval ��
  */
void W25qx_SPI_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	/* ѡ��FLASH: CS�͵�ƽ */
	W25QX_SPI_CS_LOW();

	/* ���� �� ָ�� */
	W25qx_SPI_SendByte(W25X_ReadData);

	/* ���� �� ��ַ��λ */
	W25qx_SPI_SendByte((ReadAddr & 0xFF0000) >> 16);
	/* ���� �� ��ַ��λ */
	W25qx_SPI_SendByte((ReadAddr& 0xFF00) >> 8);
	/* ���� �� ��ַ��λ */
	W25qx_SPI_SendByte(ReadAddr & 0xFF);
	
	/* ��ȡ���� */
	while (NumByteToRead--) /* while there is data to be read */
	{
		/* ��ȡһ���ֽ�*/
		*pBuffer = W25qx_SPI_SendByte(Dummy_Byte);
		/* ָ����һ���ֽڻ����� */
		pBuffer++;
	}

	/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
	W25QX_SPI_CS_HIGH();
}

 /**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
u32 W25qx_SPI_ReadID(void)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/* ��ʼͨѶ��CS�͵�ƽ */
	W25QX_SPI_CS_LOW();

	/* ����JEDECָ���ȡID */
	W25qx_SPI_SendByte(W25X_JedecDeviceID);

	/* ��ȡһ���ֽ����� */
	Temp0 = W25qx_SPI_SendByte(Dummy_Byte);

	/* ��ȡһ���ֽ����� */
	Temp1 = W25qx_SPI_SendByte(Dummy_Byte);

	/* ��ȡһ���ֽ����� */
	Temp2 = W25qx_SPI_SendByte(Dummy_Byte);

	/* ֹͣͨѶ��CS�ߵ�ƽ */
	W25QX_SPI_CS_HIGH();

	/*�����������������Ϊ�����ķ���ֵ*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}

 /**
  * @brief  ��ȡFLASH Device ID
  * @param 	��
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




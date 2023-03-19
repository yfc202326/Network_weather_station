#ifndef __W25QX_H_
#define __W25QX_H_

#include "stm32f10x.h"                  // Device header
#include <stdio.h>

//#define  sFLASH_ID              0xEF3015   //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
//#define  sFLASH_ID              0XEF4018    //W25Q64
#define  sFLASH_ID              0XEF4018   //W25Q128


#define W25QX_SPI_PageSize              256
#define W25QX_SPI_PerWritePageSize      256

/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		      0x05 
#define W25X_WriteStatusReg		      0x01 
#define W25X_ReadData			      0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	      0xAB 
#define W25X_DeviceID			      0xAB 
#define W25X_ManufactDeviceID   	  0x90 
#define W25X_JedecDeviceID		      0x9F

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF
/*命令定义-结尾*******************************/


/*SPI接口定义-开头****************************/
#define      W25QX_SPIx                        	SPI1
#define      W25QX_SPI_APBxClock_FUN          	RCC_APB2PeriphClockCmd
#define      W25QX_SPI_CLK                     	RCC_APB2Periph_SPI1

//CS(NSS)引脚 片选选普通GPIO即可
#define      W25QX_SPI_CS_APBxClock_FUN       	RCC_APB2PeriphClockCmd
#define      W25QX_SPI_CS_CLK                  	RCC_APB2Periph_GPIOA    
#define      W25QX_SPI_CS_PORT                 	GPIOA
#define      W25QX_SPI_CS_PIN                  	GPIO_Pin_4

//SCK引脚
#define      W25QX_SPI_SCK_APBxClock_FUN      	RCC_APB2PeriphClockCmd
#define      W25QX_SPI_SCK_CLK                 	RCC_APB2Periph_GPIOA   
#define      W25QX_SPI_SCK_PORT                	GPIOA   
#define      W25QX_SPI_SCK_PIN                 	GPIO_Pin_5
//MISO引脚
#define      W25QX_SPI_MISO_APBxClock_FUN     	RCC_APB2PeriphClockCmd
#define      W25QX_SPI_MISO_CLK                	RCC_APB2Periph_GPIOA    
#define      W25QX_SPI_MISO_PORT               	GPIOA 
#define      W25QX_SPI_MISO_PIN                	GPIO_Pin_6
//MOSI引脚
#define      W25QX_SPI_MOSI_APBxClock_FUN     	RCC_APB2PeriphClockCmd
#define      W25QX_SPI_MOSI_CLK                	RCC_APB2Periph_GPIOA    
#define      W25QX_SPI_MOSI_PORT               	GPIOA 
#define      W25QX_SPI_MOSI_PIN                	GPIO_Pin_7

#define  	 W25QX_SPI_CS_LOW()     			GPIO_ResetBits( W25QX_SPI_CS_PORT, W25QX_SPI_CS_PIN )
#define  	 W25QX_SPI_CS_HIGH()    			GPIO_SetBits( W25QX_SPI_CS_PORT, W25QX_SPI_CS_PIN )

/*SPI接口定义-结尾****************************/

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define W25QX_DEBUG_ON         1

#define W25QX_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define W25QX_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define W25QX_DEBUG(fmt,arg...)          do{\
                                          if(W25QX_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

void W25qx_Init(void);
u8 W25qx_SPI_SendByte(u8 byte);
u16 W25qx_SPI_SendHalfWord(u16 HalfWord);
u8 W25qx_SPI_ReadByte(void);
										  
void W25qx_SPI_WriteEnable(void);			
void W25qx_SPI_WaitForWriteEnd(void);		

void W25qx_SPI_PowerDown(void);
void W25qx_SPI_WAKEUP(void);	

void W25qx_SPI_SectorErase(u32 SectorAddr);
void W25qx_SPI_BulkErase(void);	
void W25qx_SPI_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void W25qx_SPI_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);		
void W25qx_SPI_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 W25qx_SPI_ReadID(void);
u32 W25qx_SPI_ReadDeviceID(void);	
void W25qx_SPI_StartReadSequence(u32 ReadAddr);										  
										  
#endif /* __W25QX_H_ */



/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define SD_CARD	 		0 			// Ԥ�� SD ��ʹ��
#define SPI_FLASH 		1 			// �ⲿ SPI Flash

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv)
	{
		case SD_CARD :
				break;
		case SPI_FLASH :
				if(W25qx_SPI_ReadID()==sFLASH_ID)
				{	
					//״̬����
					stat=0;
				}
				else
				{
					//״̬�쳣
					stat=STA_NOINIT;
				}
				return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) 
	{
		case SD_CARD :
				return stat;
		case SPI_FLASH :
				W25qx_Init();
				W25qx_SPI_WAKEUP();
				return disk_status(SPI_FLASH);
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;

	switch (pdrv) 
	{
		case SD_CARD :
				return res;
		case SPI_FLASH :
				//����6MB����ƫ��
				sector+=1536;		
				W25qx_SPI_BufferRead(buff,sector*4096,count*4096);
				res = RES_OK;
				return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;

	switch (pdrv) 
	{
		case SD_CARD :
				return res;

		case SPI_FLASH :
				//����6MB����ƫ��
				sector+=1536;
				//һ��Ҫ�Ȳ�������д��
				W25qx_SPI_SectorErase(sector*4096);
				W25qx_SPI_BufferWrite((u8 *)buff,sector*4096,count*4096);
				res = RES_OK;
		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (pdrv) 
	{
		case SD_CARD :
				return res;
		case SPI_FLASH :
				switch(cmd)
				{
					//�������������������10MB�����ļ�ϵͳ��д����
					case GET_SECTOR_COUNT:
							*(DWORD *)buff=2560;
							break;
					//����ÿ��������С
					case GET_SECTOR_SIZE:
							*(WORD *)buff=4096;
							break;
					//���ز�����������С��������λ������
					case GET_BLOCK_SIZE:
							*(DWORD *)buff=1;
					break;
				}
				// Process of the command for the MMC/SD card
				res = RES_OK;
				return res;
	}

	return RES_PARERR;
}

//����ʱ��
DWORD get_fattime (void)
{
	return 0;
}

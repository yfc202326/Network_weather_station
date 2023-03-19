#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__ 

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "bsp_rtc.h"
#include <string.h>
#include "esp8266_com.h"
#include "bsp_w25qx.h"
#include "ff.h"

/*---------------------------------------------------文件系统相关变量---------------------------------------------------------*/

extern FATFS fs;											//FatFs文件系统对象
extern FATFS *pfs;										//指向FatFs文件系统对象的指针
extern FIL fnew; 											//文件对象
extern FRESULT res_FR; 									//文件操作结果
extern UINT nbw; 											//Number of bytes written
extern UINT nbr;											//Number of bytes read
extern BYTE work[FF_MAX_SS]; 								//Work area (larger is better for processing time)（格式化工作区）
extern DWORD fre_clust, fre_sect, tot_sect;				//空闲簇，空闲扇区，总扇区

extern BYTE fReadbuffer[1024];	 						//读缓冲区
extern BYTE fWriteBuffer[21]; 		//写缓冲区

/*------------------------------------------------------EBDLINE-------------------------------------------------------------*/

/*---------------------------------------------Esp8266接收数据相关结构定义----------------------------------------------------*/

//es8266天气数据结构体定义
typedef struct esp8266_rec_weather
{
	char name[20];
	char text[20];
	char wea_code[10];
	char temperature[10];
	char tem_high[10];
	char tem_low[10];
	char humidity[10];
}rec_weather;

//esp8266时间和天气数据接收结构体定义
typedef struct esp8266_recive 
{
	char rec_time[30];
	rec_weather Dat_weather;
	
}Esp8266_Recive;

//当前天气定结构体定义
struct cur_weather
{
	char cur_city[20];
	char cur_wea[20];
	uint8_t cur_wea_code;
	int8_t cur_tem;
	int8_t tem_high;
	int8_t tem_low;
	uint8_t cur_humid;
};

/*------------------------------------------------------EBDLINE-------------------------------------------------------------*/


//外部flash字库地址
#define ASCLL_H16_BASE_ADDR		(4*1024)
#define ASCLL_H32_BASE_ADDR		(8*1024)
#define ASCLL_H48_BASE_ADDR		(20*1024)
#define GB2312_H16_BASE_ADDR	(40*1024)
#define GB2312_H24_BASE_ADDR	(300*1024)
#define GB2312_H32_BASE_ADDR	(880*1024)

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
#define AQBLUE           0x67FC //水蓝
#define DSBLUE           0x5FF  //深天蓝
#define PEABLUE          0x411  //孔雀蓝
#define ORANGE           0xFD20 //橙色
#define MINT             0x14C5 //薄荷绿
#define SEAGREEN         0x2C4A //海绿
#define BRIGREEN         0x67E0 //明绿

#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

//LCD屏幕的宽和高
#define LCD_W 240
#define LCD_H 240

//--------------------LCD显示屏端口定义--------------------
//SCL(注意:PB4需要重映射，需开启AFIO时钟)
#define 	LCD_GPIO_SCL_CLK			(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define 	LCD_GPIO_SCL_PORT			GPIOB
#define     LCD_GPIO_SCL_PIN			GPIO_Pin_4
//SDA
#define 	LCD_GPIO_SDA_CLK			RCC_APB2Periph_GPIOB
#define 	LCD_GPIO_SDA_PORT			GPIOB
#define     LCD_GPIO_SDA_PIN			GPIO_Pin_5
//RES
#define 	LCD_GPIO_RES_CLK			RCC_APB2Periph_GPIOB
#define 	LCD_GPIO_RES_PORT			GPIOB
#define     LCD_GPIO_RES_PIN			GPIO_Pin_6
//DC
#define 	LCD_GPIO_DC_CLK				RCC_APB2Periph_GPIOB
#define 	LCD_GPIO_DC_PORT			GPIOB
#define     LCD_GPIO_DC_PIN				GPIO_Pin_7
//BLK
#define 	LCD_GPIO_BLK_CLK			RCC_APB2Periph_GPIOB
#define 	LCD_GPIO_BLK_PORT			GPIOB
#define     LCD_GPIO_BLK_PIN			GPIO_Pin_8

//--------------------LCD端口操作定义-------------------- 
//SCL(SCL=SCLK)
#define 	LCD_SCLK_Clr() 		GPIO_ResetBits(LCD_GPIO_SCL_PORT,LCD_GPIO_SCL_PIN)
#define 	LCD_SCLK_Set() 		GPIO_SetBits(LCD_GPIO_SCL_PORT,LCD_GPIO_SCL_PIN)
//SDA(SDA=MOSI)
#define 	LCD_MOSI_Clr() 		GPIO_ResetBits(LCD_GPIO_SDA_PORT,LCD_GPIO_SDA_PIN)
#define 	LCD_MOSI_Set() 		GPIO_SetBits(LCD_GPIO_SDA_PORT,LCD_GPIO_SDA_PIN)
//RES
#define 	LCD_RES_Clr()  		GPIO_ResetBits(LCD_GPIO_RES_PORT,LCD_GPIO_RES_PIN)
#define 	LCD_RES_Set()  		GPIO_SetBits(LCD_GPIO_RES_PORT,LCD_GPIO_RES_PIN)
//DC
#define 	LCD_DC_Clr()   		GPIO_ResetBits(LCD_GPIO_DC_PORT,LCD_GPIO_DC_PIN)
#define 	LCD_DC_Set()   		GPIO_SetBits(LCD_GPIO_DC_PORT,LCD_GPIO_DC_PIN)
//BLK
#define 	LCD_BLK_Clr()  		GPIO_ResetBits(LCD_GPIO_BLK_PORT,LCD_GPIO_BLK_PIN)
#define 	LCD_BLK_Set()  		GPIO_SetBits(LCD_GPIO_BLK_PORT,LCD_GPIO_BLK_PIN)

void LCD_Init(void);

void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);//指定区域填充颜色
void LCD_DrawPoint(u16 x,u16 y,u16 color);//在指定位置画一个点
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);//在指定位置画一条线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//在指定位置画一个矩形
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);//在指定位置画一个圆

void LCD_ShowChinese16x16(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个32x32汉字
void LCD_ShowChinese(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示汉字串

void LCD_ShowChar(u16 x,u16 y,char num,u16 fc,u16 bc,u8 sizey,u8 mode);//显示一个字符
void LCD_ShowString(u16 x,u16 y,char *p,u16 fc,u16 bc,u8 sizey,u8 mode);//显示字符串

//注意：仅当中文字体与英文字体高度（16 32）相同时能使用此函数
void LCD_ShowText(u16 x,u16 y,char *p,u16 fc,u16 bc,u8 sizey,u8 mode);//显示任意文本

u32 mypow(u8 m,u8 n);//求幂
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);//显示整数变量
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);//显示两位小数变量

void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);//显示图片
void LCD_Show_Wea_Picture(u16 x,u16 y,u16 length,u16 width,u8 pic_code);//显示天气图片
void LCD_ShowIcon(u16 x,u16 y,u16 length,u16 width,const u8 icon[],u16 fc,u16 bc);//显示图标
void LCD_Show_Wifi_Icon(u16 x,u16 y,u16 length,u16 width,u16 fc,u16 bc);//显示wifi图标

void LCD_Display_power(uint8_t wifi_link);
void LCD_Display_City(struct cur_weather *currweather);
void LCD_Display_weather(struct cur_weather *currweather);
void LCD_Display_Time(struct rtc_time *time);
void LCD_Display_Tem_Hum(struct cur_weather *currweather);

void Esp8266_Connect_Time(struct rtc_time *tm);
void Esp8266_Connect_Weather(struct cur_weather *currweather);


#endif /* __LCD_DISPLAY_H__ */







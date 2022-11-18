#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__ 

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "lcdfont.h"

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

//LCD屏幕的宽和高
#define LCD_W 128
#define LCD_H 128

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
//CS
#define 	LCD_GPIO_CS_CLK				RCC_APB2Periph_GPIOB
#define 	LCD_GPIO_CS_PORT			GPIOB
#define     LCD_GPIO_CS_PIN				GPIO_Pin_8
//BLK
#define 	LCD_GPIO_BLK_CLK			RCC_APB2Periph_GPIOB
#define 	LCD_GPIO_BLK_PORT			GPIOB
#define     LCD_GPIO_BLK_PIN			GPIO_Pin_9

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
//CS 		     
#define 	LCD_CS_Clr()  		GPIO_ResetBits(LCD_GPIO_CS_PORT,LCD_GPIO_CS_PIN)
#define 	LCD_CS_Set()   		GPIO_SetBits(LCD_GPIO_CS_PORT,LCD_GPIO_CS_PIN)
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

void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);//显示一个字符
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode);//显示字符串

u32 mypow(u8 m,u8 n);//求幂
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);//显示整数变量
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);//显示两位小数变量

void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);//显示图片
void LCD_ShowIcon(u16 x,u16 y,u16 length,u16 width,const u8 icon[],u16 fc,u16 bc);//显示图标

void LCD_Display_power(void);
void LCD_Display_weather(void);
void LCD_Display_City(void);
void LCD_Display_Time(u8 hour,u8 min);
void LCD_Display_Tem_Hum(void);


#endif /* __LCD_DISPLAY_H__ */







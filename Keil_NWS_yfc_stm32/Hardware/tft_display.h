#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__ 

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "bsp_rtc.h"
#include <string.h>
#include "esp8266_com.h"
#include "bsp_w25qx.h"
#include "ff.h"

/*---------------------------------------------------�ļ�ϵͳ��ر���---------------------------------------------------------*/

extern FATFS fs;											//FatFs�ļ�ϵͳ����
extern FATFS *pfs;										//ָ��FatFs�ļ�ϵͳ�����ָ��
extern FIL fnew; 											//�ļ�����
extern FRESULT res_FR; 									//�ļ��������
extern UINT nbw; 											//Number of bytes written
extern UINT nbr;											//Number of bytes read
extern BYTE work[FF_MAX_SS]; 								//Work area (larger is better for processing time)����ʽ����������
extern DWORD fre_clust, fre_sect, tot_sect;				//���дأ�����������������

extern BYTE fReadbuffer[1024];	 						//��������
extern BYTE fWriteBuffer[21]; 		//д������

/*------------------------------------------------------EBDLINE-------------------------------------------------------------*/

/*---------------------------------------------Esp8266����������ؽṹ����----------------------------------------------------*/

//es8266�������ݽṹ�嶨��
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

//esp8266ʱ����������ݽ��սṹ�嶨��
typedef struct esp8266_recive 
{
	char rec_time[30];
	rec_weather Dat_weather;
	
}Esp8266_Recive;

//��ǰ�������ṹ�嶨��
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


//�ⲿflash�ֿ��ַ
#define ASCLL_H16_BASE_ADDR		(4*1024)
#define ASCLL_H32_BASE_ADDR		(8*1024)
#define ASCLL_H48_BASE_ADDR		(20*1024)
#define GB2312_H16_BASE_ADDR	(40*1024)
#define GB2312_H24_BASE_ADDR	(300*1024)
#define GB2312_H32_BASE_ADDR	(880*1024)

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
#define AQBLUE           0x67FC //ˮ��
#define DSBLUE           0x5FF  //������
#define PEABLUE          0x411  //��ȸ��
#define ORANGE           0xFD20 //��ɫ
#define MINT             0x14C5 //������
#define SEAGREEN         0x2C4A //����
#define BRIGREEN         0x67E0 //����

#define USE_HORIZONTAL 1  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����

//LCD��Ļ�Ŀ�͸�
#define LCD_W 240
#define LCD_H 240

//--------------------LCD��ʾ���˿ڶ���--------------------
//SCL(ע��:PB4��Ҫ��ӳ�䣬�迪��AFIOʱ��)
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

//--------------------LCD�˿ڲ�������-------------------- 
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

void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);//ָ�����������ɫ
void LCD_DrawPoint(u16 x,u16 y,u16 color);//��ָ��λ�û�һ����
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);//��ָ��λ�û�һ����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//��ָ��λ�û�һ������
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);//��ָ��λ�û�һ��Բ

void LCD_ShowChinese16x16(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ����16x16����
void LCD_ShowChinese24x24(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ����24x24����
void LCD_ShowChinese32x32(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ����32x32����
void LCD_ShowChinese(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ���ִ�

void LCD_ShowChar(u16 x,u16 y,char num,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾһ���ַ�
void LCD_ShowString(u16 x,u16 y,char *p,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ�ַ���

//ע�⣺��������������Ӣ������߶ȣ�16 32����ͬʱ��ʹ�ô˺���
void LCD_ShowText(u16 x,u16 y,char *p,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ�����ı�

u32 mypow(u8 m,u8 n);//����
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);//��ʾ��������
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);//��ʾ��λС������

void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);//��ʾͼƬ
void LCD_Show_Wea_Picture(u16 x,u16 y,u16 length,u16 width,u8 pic_code);//��ʾ����ͼƬ
void LCD_ShowIcon(u16 x,u16 y,u16 length,u16 width,const u8 icon[],u16 fc,u16 bc);//��ʾͼ��
void LCD_Show_Wifi_Icon(u16 x,u16 y,u16 length,u16 width,u16 fc,u16 bc);//��ʾwifiͼ��

void LCD_Display_power(uint8_t wifi_link);
void LCD_Display_City(struct cur_weather *currweather);
void LCD_Display_weather(struct cur_weather *currweather);
void LCD_Display_Time(struct rtc_time *time);
void LCD_Display_Tem_Hum(struct cur_weather *currweather);

void Esp8266_Connect_Time(struct rtc_time *tm);
void Esp8266_Connect_Weather(struct cur_weather *currweather);


#endif /* __LCD_DISPLAY_H__ */







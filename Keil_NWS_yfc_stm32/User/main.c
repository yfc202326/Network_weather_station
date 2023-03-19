/*
网络气象站:
能够显示时间、天气、温度、湿度
MCU:stm32f103rct6
主频:72MHz
*/
#include "Indicator_LED.h"
#include "bsp_systick.h"
#include "Delay.h"
#include "tft_display.h"
#include "bsp_key.h"
#include "bsp_w25qx.h"

/*---------------------------------------------------文件系统相关变量---------------------------------------------------------*/

FATFS fs;											//FatFs文件系统对象
FATFS *pfs=&fs;										//指向FatFs文件系统对象的指针
FIL fnew; 											//文件对象
FRESULT res_FR; 									//文件操作结果
UINT nbw; 											//Number of bytes written
UINT nbr;											//Number of bytes read
BYTE work[FF_MAX_SS]; 								//Work area (larger is better for processing time)（格式化工作区）
DWORD fre_clust, fre_sect, tot_sect;				//空闲簇，空闲扇区，总扇区

BYTE fReadbuffer[1024]= {0};	 					//读缓冲区
BYTE fWriteBuffer[] = "这是一个测试文件！\r\n"; 	 	//写缓冲区

/*--------------------------------------------------------------------------------------------------------------------------*/

/*时间结构体，默认时间2000-01-01 00:00:00*/
struct rtc_time systmtime=
{
23,12,6,1,1,2023,0
};

/*天气结构体，默认地区 beijing*/
struct cur_weather currweather=
{
	"Guangyuan",
	"sunny",
	0,
	0,
	0,
	0,
	0,
};

extern __IO uint32_t TimeDisplay ;

int main()
{
	//指示灯初始化
	Indicator_LED_GPIO_Init();	
	//串口初始化
	USART_Config();	
	//W25Q128初始化
	W25qx_Init();
	//Esp8266初始化
	Esp8266_Init();
	//按键初始化
	Key_GPIO_Config();
	//RTC时钟初始化
	RTC_NVIC_Config();
	RTC_CheckAndConfig(&systmtime);
	//TFT_LCD初始化
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
	//电量及wifi连接状态
	LCD_Display_power(0);	
	//连接wifi自动获取数据
	Esp8266_Connect_Weather(&currweather);
	Esp8266_Connect_Time(&systmtime);
	//天气显示
	LCD_Display_weather(&currweather);//天气图标	
	LCD_Display_City(&currweather);//城市及天气	
	LCD_Display_Tem_Hum(&currweather);//温度及湿度
	//时间显示
	LCD_Display_Time(&systmtime);
	while(1)
	{
		/* 每过1s */
	    if (TimeDisplay == 1)
	    {
			/* Display current time */
			Time_Display( RTC_GetCounter(),&systmtime); 
			LCD_Display_Time(&systmtime);			
			TimeDisplay = 0;
	    }
		/* 按键3按下，串口设置时间 */
		if(Key_Scan( KEY3_GPIO_PORT,KEY3_GPIO_PIN ) == KEY_ON )
		{
			struct rtc_time set_time;
			
			//LED灯状态转换
			Indicator_LED_TOGGLE;
			
			//使用串口接收设置的时间，输入数字时注意末尾要加回车
			Time_Regulate_Get(&set_time);
			//用接收到的时间设置RTC
			Time_Adjust(&set_time);				
			//向备份寄存器写入标志
			BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);					
		}
		/* 按键4按下，wifi自动设置时间 */
		if( Key_Scan(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == KEY_ON)
		{
			//LED灯状态转换
			Indicator_LED_TOGGLE;

			//wifi自动获取时间
			Esp8266_Connect_Time(&systmtime);

			//天气显示
			LCD_Display_weather(&currweather);//天气图标	
			LCD_Display_City(&currweather);	
			LCD_Display_Tem_Hum(&currweather);//温度及湿度
		}
	}
	return 0;
}

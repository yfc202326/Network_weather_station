/*
��������վ:
�ܹ���ʾʱ�䡢�������¶ȡ�ʪ��
MCU:stm32f103rct6
��Ƶ:72MHz
*/
#include "Indicator_LED.h"
#include "bsp_systick.h"
#include "Delay.h"
#include "tft_display.h"
#include "bsp_key.h"
#include "bsp_w25qx.h"

/*---------------------------------------------------�ļ�ϵͳ��ر���---------------------------------------------------------*/

FATFS fs;											//FatFs�ļ�ϵͳ����
FATFS *pfs=&fs;										//ָ��FatFs�ļ�ϵͳ�����ָ��
FIL fnew; 											//�ļ�����
FRESULT res_FR; 									//�ļ��������
UINT nbw; 											//Number of bytes written
UINT nbr;											//Number of bytes read
BYTE work[FF_MAX_SS]; 								//Work area (larger is better for processing time)����ʽ����������
DWORD fre_clust, fre_sect, tot_sect;				//���дأ�����������������

BYTE fReadbuffer[1024]= {0};	 					//��������
BYTE fWriteBuffer[] = "����һ�������ļ���\r\n"; 	 	//д������

/*--------------------------------------------------------------------------------------------------------------------------*/

/*ʱ��ṹ�壬Ĭ��ʱ��2000-01-01 00:00:00*/
struct rtc_time systmtime=
{
23,12,6,1,1,2023,0
};

/*�����ṹ�壬Ĭ�ϵ��� beijing*/
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
	//ָʾ�Ƴ�ʼ��
	Indicator_LED_GPIO_Init();	
	//���ڳ�ʼ��
	USART_Config();	
	//W25Q128��ʼ��
	W25qx_Init();
	//Esp8266��ʼ��
	Esp8266_Init();
	//������ʼ��
	Key_GPIO_Config();
	//RTCʱ�ӳ�ʼ��
	RTC_NVIC_Config();
	RTC_CheckAndConfig(&systmtime);
	//TFT_LCD��ʼ��
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
	//������wifi����״̬
	LCD_Display_power(0);	
	//����wifi�Զ���ȡ����
	Esp8266_Connect_Weather(&currweather);
	Esp8266_Connect_Time(&systmtime);
	//������ʾ
	LCD_Display_weather(&currweather);//����ͼ��	
	LCD_Display_City(&currweather);//���м�����	
	LCD_Display_Tem_Hum(&currweather);//�¶ȼ�ʪ��
	//ʱ����ʾ
	LCD_Display_Time(&systmtime);
	while(1)
	{
		/* ÿ��1s */
	    if (TimeDisplay == 1)
	    {
			/* Display current time */
			Time_Display( RTC_GetCounter(),&systmtime); 
			LCD_Display_Time(&systmtime);			
			TimeDisplay = 0;
	    }
		/* ����3���£���������ʱ�� */
		if(Key_Scan( KEY3_GPIO_PORT,KEY3_GPIO_PIN ) == KEY_ON )
		{
			struct rtc_time set_time;
			
			//LED��״̬ת��
			Indicator_LED_TOGGLE;
			
			//ʹ�ô��ڽ������õ�ʱ�䣬��������ʱע��ĩβҪ�ӻس�
			Time_Regulate_Get(&set_time);
			//�ý��յ���ʱ������RTC
			Time_Adjust(&set_time);				
			//�򱸷ݼĴ���д���־
			BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);					
		}
		/* ����4���£�wifi�Զ�����ʱ�� */
		if( Key_Scan(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == KEY_ON)
		{
			//LED��״̬ת��
			Indicator_LED_TOGGLE;

			//wifi�Զ���ȡʱ��
			Esp8266_Connect_Time(&systmtime);

			//������ʾ
			LCD_Display_weather(&currweather);//����ͼ��	
			LCD_Display_City(&currweather);	
			LCD_Display_Tem_Hum(&currweather);//�¶ȼ�ʪ��
		}
	}
	return 0;
}

#include "Indicator_LED.h"
#include "bsp_systick.h"
#include "Delay.h"
#include "tft_display.h"

int main()
{
	uint8_t k=10;
	Indicator_LED_GPIO_Init();
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
//	LCD_DrawRectangle(0,0,127,127,RED);

	//����������״̬
	LCD_Display_power();	
	//����ͼ��
	LCD_Display_weather();	
	//���м���������
	LCD_Display_City();	
	//ʱ��
	LCD_Display_Time(20,k);
	//�¶ȼ�ʪ��
	LCD_Display_Tem_Hum();
	
	while(1)
	{
		Indicator_LED(ON);
		Delay_ms(500);
		Indicator_LED(OFF);
		Delay_ms(500);
		k++;
		LCD_Display_Time(20,k);	
	}
	return 0;
}

#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__ 

#include "bsp_usart.h"
#include "bsp_date.h"
#include "bsp_calendar.h"
#include "Indicator_LED.h"

//是否使用LCD显示日期
//#define USE_LCD_DISPLAY

//使用LSE外部时钟 或 LSI内部时钟
//#define RTC_CLOCK_SOURCE_LSE      
#define RTC_CLOCK_SOURCE_LSI

#define RTC_BKP_DRX          BKP_DR1
// 写入到备份寄存器的数据宏定义
#define RTC_BKP_DATA         0xA5A5

//北京时间的时区秒数差(8h==8*60*60s)
#define TIME_ZOOM						(8*60*60)

void RTC_NVIC_Config(void);
void Time_Adjust(struct rtc_time *tm);
void RTC_CheckAndConfig(struct rtc_time *tm);
void Time_Regulate_Get(struct rtc_time *tm);
void Time_Display(uint32_t TimeVar,struct rtc_time *tm);
void Time_Show(struct rtc_time *tm);

#endif /* __BSP_RTC_H__ */



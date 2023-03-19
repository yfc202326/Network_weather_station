#ifndef __ESP8266_COM_H__
#define __ESP8266_COM_H__ 

#include "stm32f10x.h"                  // Device header
#include "Indicator_LED.h"
#include "bsp_usart.h"

// 串口1-USART1
#define  ESP_USARTx                   USART2
#define  ESP_USART_CLK                RCC_APB1Periph_USART2
#define  ESP_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  ESP_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  ESP_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  ESP_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  ESP_USART_TX_GPIO_PORT       GPIOA   
#define  ESP_USART_TX_GPIO_PIN        GPIO_Pin_2
#define  ESP_USART_RX_GPIO_PORT       GPIOA
#define  ESP_USART_RX_GPIO_PIN        GPIO_Pin_3

#define  ESP_USART_IRQ                USART2_IRQn
#define  ESP_USART_IRQHandler         USART2_IRQHandler

void Esp8266_Init(void);
void Esp8266_SendByte(USART_TypeDef *pUSARTx, uint8_t Byte);
void Esp8266_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t HalfWord);
void Esp8266_SendArray(USART_TypeDef *pUSARTx, uint8_t *array,uint8_t num);
void Esp8266_SendString(USART_TypeDef *pUSARTx, char *str);
void Esp8266_SendCmd(char *cmd);

#endif /* __ESP8266_COM_H__ */


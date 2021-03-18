
#ifndef _usart_h
#define _usart_h

#ifdef __cplusplus
extern "C" {
#endif
	
#include "hk32f030m.h"
#include "hk32f030m_gpio.h"
#include "hk32f030m_usart.h"
	
#define  USART1_TX_PORT         GPIOD
#define  USART1_TX_PIN          GPIO_Pin_1
#define  USART1_TX_IO_CLK_EN()  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE)
	
#define  USART1_RX_PORT         GPIOD
#define  USART1_RX_PIN          GPIO_Pin_6
#define  USART1_RX_IO_CLK_EN()  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE)
	
void USART_Configurature(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _usart_h */

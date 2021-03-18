
/**************************************************************************************************
* @file      usart_async_tx_no_int_rx_rxneint.c
* @brief     异步串口通信例程, 通过查询TXE标志发送数据,通过RXNE中断接收数据,当中断接收到数据后会将
*            收到的数据原样通过TX引脚发送出去
* @author    veiko.c
* @date      20200202
**************************************************************************************************/

#include "usart.h"

static void USART_GPIO_Configurature(void);

void USART_Configurature(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_GPIO_Configurature();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	
}

static void USART_GPIO_Configurature(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	USART1_TX_IO_CLK_EN();
	USART1_RX_IO_CLK_EN();
	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(USART1_TX_PORT,GPIO_PinSource1,GPIO_AF_1);
	
	m_gpio.GPIO_Pin = USART1_RX_PIN;
	GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(USART1_RX_PORT,GPIO_PinSource6,GPIO_AF_1);
}



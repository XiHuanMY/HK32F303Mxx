/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hk32f030m.h"
#include "hk32f030m_gpio.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "ws2812.h"
#include "key.h"
#include "dt12.h"
#include "lcd.h"

int main(void)/* Infinite loop */
{
	RCC_MCOConfig(RCC_MCOSource_HSI,RCC_MCOPrescaler_1);
	WS2812Init();
	KEYInit();
	DHT12Init();
	LCDInit();
	USART_Configurature();
	printf("\r\nhk32 test\r\n");
	
	P16x16Str(20,1,"12"); //显示温度
	P16x16Str(20,3,"02"); //显示湿度
	P8x16Str(100,1,"<");	//显示 %
	P8x16Str(100,3,";");	//显示 ℃
	while (1)
  {
		Read_Temp();
		RGB_Ws2812();
  }
}

int fputc(int ch,FILE *f)
{

	USART_SendData(USART1, (uint8_t) ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {}		
  return ch;
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */



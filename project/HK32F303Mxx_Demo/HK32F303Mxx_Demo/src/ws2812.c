#include "main.h"
#include "hk32f030m.h"
#include "hk32f030m_gpio.h"
#include "ws2812.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "dt12.h"
#define RED_INDEX   1
#define GREEN_INDEX 0
#define BLUE_INDEX  2
uint32_t RGB_i;
uint32_t color = 0x000000;
void WS2812Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void set_color_5(uint32_t color)
{
			Ws2812b_Write(color);
			Ws2812b_Write(color);
			Ws2812b_Write(color);
			Ws2812b_Write(color);
			Ws2812b_Write(color);		
}
void set_color(uint32_t color[])
{
			Ws2812b_Write(color[0]);
			Ws2812b_Write(color[1]);
			Ws2812b_Write(color[2]);
			Ws2812b_Write(color[3]);
			Ws2812b_Write(color[4]);		
			softWareDelay();
}


void Ws2812b_Write(uint32_t color)
{
	for(int j=0;j<24;j++)
	{
		if(color & 0x800000)
			 Ws2812b_Write1();
		else
			 Ws2812b_Write0();
	 color = color << 1;
	}
}

void Ws2812b_Write1(void)
{
	int j=10;
	 GPIOC->BSRR = 0x08;
	 while(j--);
	 GPIOC->BRR = 0x08;
	
}
void Ws2812b_Write0(void)
{
	 GPIOC->BSRR = 0x08;
	 GPIOC->BRR = 0x08;
}
extern int RGB_ON;
void RGB_Ws2812(void)
{
		
    uint8_t state = 1;
    uint8_t RGBList[3] = {255, 0, 0};	//从红色开始彩虹灯
    
    while(1)
    {
        if (RGB_ON ==0)
        {
					set_color_5(0x00000000);
            break;
        }
        for(uint8_t cnt = 0; cnt < (255-1); cnt++)	//cnt为uint8_t时此处如果条件为<255将死循环，不会有全光谱彩虹灯
        {
            switch (state)
            {
              case 1:
                RGBList[GREEN_INDEX] = cnt;
                break;
              case 2:
                RGBList[RED_INDEX] = 255 - cnt;
                break;
              case 3:
                RGBList[BLUE_INDEX] = cnt;
                break;
              case 4:
                RGBList[GREEN_INDEX] = 255 - cnt;
                break;
              case 5:
                RGBList[RED_INDEX] = cnt;
                break;
              case 6:
                RGBList[BLUE_INDEX] = 255 - cnt;
                break;
              default:
                break;
            }
						color  |= RGBList[RED_INDEX] ;
						color = color<<8;
						color  |= RGBList[GREEN_INDEX] ;
						color = color<<8;
						color  |= RGBList[BLUE_INDEX] ;
		
            delay_ms(100);
            set_color_5(color);
						color = 0;
        }
        if (state < 6)
            state++;
        else
            state = 1;
       delay_ms(50);
    }
}

static void softWareDelay(void)
{
	uint16_t i;
	uint16_t j;
	
	for(i=0;i<300;i++)
	{
		for(j=0;j<200;j++)
		{
			__NOP();
		}
	}
}












#include "main.h"
#include "hk32f030m.h"
#include "hk32f030m_gpio.h"
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "dt12.h"
#include "font.h"
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 |GPIO_Pin_4;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(LCD_EN_PORT,LCD_EN_PIN);
	GPIO_SetBits(LCD_DC_PORT,LCD_DC_PIN);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = LCD_MOSI_PIN;
	GPIO_Init(LCD_MOSI_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(LCD_MOSI_PORT, GPIO_PinSource6, GPIO_AF_2);
  	/* config the CLK PIN */
	GPIO_InitStructure.GPIO_Pin = LCD_SCK_PIN;
	GPIO_Init(LCD_SCK_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(LCD_SCK_PORT, GPIO_PinSource2, GPIO_AF_2);
	
	 /* SPI configuration -------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_SSOutputCmd(SPI1, ENABLE);
	/* Enable the SPI peripheral */
	SPI_Cmd(SPI1, ENABLE);

}

void LCDInit(void)
{

	LCD_GPIOInit();

	GPIO_ResetBits(LCD_CS_PORT,LCD_CS_PIN);
	GPIO_ResetBits(LCD_RES_PORT,LCD_RES_PIN);
	delay_ms(200);
	GPIO_SetBits(LCD_RES_PORT,LCD_RES_PIN);
	LCD_WrCmd(0xae);//--turn off oled panel
	LCD_WrCmd(0x00);//---set low column address
	LCD_WrCmd(0x10);//---set high column address
	LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	LCD_WrCmd(0x81);//--set contrast control register
	LCD_WrCmd(0xcf);// Set SEG Output Current Brightness
	LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	LCD_WrCmd(0xa6);//--set normal display
	LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	LCD_WrCmd(0x3f);//--1/64 duty
	LCD_WrCmd(0xd3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
	LCD_WrCmd(0x00);//-not offset
	LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	LCD_WrCmd(0xd9);//--set pre-charge period
	LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	LCD_WrCmd(0xda);//--set com pins hardware configuration
	LCD_WrCmd(0x12);
	LCD_WrCmd(0xdb);//--set vcomh
	LCD_WrCmd(0x40);//Set VCOM Deselect Level
	LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	LCD_WrCmd(0x02);//
	LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
	LCD_WrCmd(0x14);//--set(0x10) disable
	LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
	LCD_WrCmd(0xaf);//--turn on oled panel
	
	LCD_Fill(0x00); //初始成黑屏
	//设置偏移值
	LCD_Set_Pos(0,0);
}

void LCD_WrCmd(uint8_t cmd)
{
	GPIO_ResetBits(LCD_DC_PORT,LCD_DC_PIN);
	SPI_SendData8(SPI1,cmd);
}
void LCD_WrDat(uint8_t dat)
{
	GPIO_SetBits(LCD_DC_PORT,LCD_DC_PIN);
	SPI_SendData8(SPI1,dat);
}

void LCD_Set_Pos(uint8_t x, uint8_t y)
{ 
    LCD_WrCmd(0xb0+y);
    LCD_WrCmd(((x&0xf0)>>4)|0x10);
    LCD_WrCmd((x&0x0f)|0x00);
}

void LCD_Fill(uint8_t bmp_dat)
{
    uint8_t y,x;

    for(y=0;y<8;y++)
    {
				LCD_WrCmd(0xb0+y);
			  LCD_WrCmd(0x01);
				LCD_WrCmd(0x10);

        for(x=0;x<128;x++)
            LCD_WrDat(bmp_dat);
    }
}



void P8x16Str(uint8_t x, uint8_t y,uint8_t ch[])
{
	uint8_t c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		if(ch[j] ==0x2e)
			c =10;
		else
			c =ch[j]-0x30;
		
		LCD_Set_Pos(x,y);
		for(i=0;i<8;i++)
			LCD_WrDat(font[c][i]);
		LCD_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
			LCD_WrDat(font[c][i+8]);
		x+=8;
		j++;
	}
}

void P16x16Str(uint8_t x, uint8_t y,uint8_t ch[])
{
	uint8_t c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j] - 0x30;
		LCD_Set_Pos(x,y);
		for(i=0;i<16;i++)
			LCD_WrDat(font1[c][i]);
		LCD_Set_Pos(x,y+1);
		for(i=0;i<16;i++)
			LCD_WrDat(font1[c][i+16]);
		x+=16;
		j++;
	}
}



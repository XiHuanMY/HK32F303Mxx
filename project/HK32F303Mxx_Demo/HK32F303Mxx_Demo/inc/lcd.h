
#include "hk32f030m.h"
#include "hk32f030m_gpio.h"

#define  LCD_SCK_PORT         GPIOA
#define  LCD_SCK_PIN          GPIO_Pin_2

#define  LCD_DC_PORT          GPIOD
#define  LCD_DC_PIN           GPIO_Pin_2
#define  LCD_RES_PORT         GPIOD
#define  LCD_RES_PIN          GPIO_Pin_3
#define  LCD_EN_PORT          GPIOD
#define  LCD_EN_PIN           GPIO_Pin_4

#define  LCD_CS_PORT          GPIOC
#define  LCD_CS_PIN           GPIO_Pin_4
#define  LCD_MOSI_PORT        GPIOC
#define  LCD_MOSI_PIN         GPIO_Pin_6
void LCD_GPIOInit(void);
void LCDInit(void);
void LCD_WrCmd(uint8_t cmd);
void LCD_WrDat(uint8_t dat);
void LCD_Set_Pos(uint8_t x, uint8_t y);
void LCD_Fill(uint8_t bmp_dat);
void P8x16Str(uint8_t x, uint8_t y,uint8_t ch[]);
void P16x16Str(uint8_t x, uint8_t y,uint8_t ch[]);



#include "hk32f030m.h"
#include "hk32f030m_gpio.h"
#include "hk32f030m_i2c.h"
	
#define  I2C_CLK_PORT         GPIOB
#define  I2C_CLK_PIN          GPIO_Pin_4

#define  I2C_DAT_PORT         GPIOC
#define  I2C_DAT_PIN          GPIO_Pin_5

typedef enum
{
  I2C_OK                                          = 0,
  I2C_FAIL                                        = 1
}I2C_Status;

/* Exported constants --------------------------------------------------------*/
#define I2C_TIMING                                0x00210507
#define I2C_TIMEOUT                               0x1000



void DHT12Init(void);
I2C_Status I2C1_Read_NBytes(uint8_t driver_Addr , uint8_t number_Bytes, uint8_t *read_Buffer); 
I2C_Status I2C1_Write_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *write_Buffer); 
void delay_us(uint16_t time);
void delay_ms(uint16_t time);
void Read_Temp(void);

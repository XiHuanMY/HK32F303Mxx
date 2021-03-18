#include "main.h"
#include "hk32f030m.h"
#include "hk32f030m_gpio.h"
#include "dt12.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "lcd.h"

uint32_t I2C_Timeout;

void DHT12Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
  RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = I2C_CLK_PIN ;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(I2C_CLK_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(I2C_CLK_PORT,GPIO_PinSource4,GPIO_AF_0);
	
	GPIO_InitStructure.GPIO_Pin = I2C_DAT_PIN;
	GPIO_Init(I2C_DAT_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(I2C_DAT_PORT,GPIO_PinSource5,GPIO_AF_0);
	
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x01;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_Timing = 0x002105070;//0x30E32E44;
  I2C_Init(I2C1, &I2C_InitStructure);
  
  I2C_Cmd(I2C1, ENABLE);	
}

I2C_Status I2C1_Read_NBytes(uint8_t driver_Addr, uint8_t number_Bytes, uint8_t *read_Buffer)
{
  uint8_t read_Num;
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
  
  I2C_TransferHandling(I2C1, driver_Addr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
  
 
  
  I2C_TransferHandling(I2C1, driver_Addr, number_Bytes,  I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  
  for(read_Num = 0; read_Num < number_Bytes; read_Num++)
  {
    I2C_Timeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
    {
      if((I2C_Timeout--) == 0)
      {
        return I2C_FAIL;
      }
    }
    
    read_Buffer[read_Num] = I2C_ReceiveData(I2C1);
  }
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }

  return I2C_OK;
}

/**
  * @brief  从I2C1的总线上的某一器件的某一起始地址中读取一定字节的数据到数组中
  * @param  driver_Addr：I2C器件地址
  * @param  start_Addr：起始字节地址
  * @param  number_Bytes：要读取的字节数量（小于一页）
  * @param  write_Buffer：存放读取数据的数组指针
  * @retval 是否读取成功
  */
I2C_Status I2C1_Write_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *write_Buffer)
{
  uint8_t write_Num;
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
  
  I2C_TransferHandling(I2C1, driver_Addr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
  
  I2C_SendData(I2C1, start_Addr);
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }

  I2C_TransferHandling(I2C1, driver_Addr, number_Bytes, I2C_AutoEnd_Mode, I2C_No_StartStop);
  
  for(write_Num = 0; write_Num < number_Bytes; write_Num++)
  {
    I2C_Timeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
    {
      if((I2C_Timeout--) == 0)
      {
        return I2C_FAIL;
      }
    }
    
    I2C_SendData(I2C1, write_Buffer[write_Num]);
  }

  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
  
  
  return I2C_OK;
}

void delay_us(uint16_t time)
{
			time *=4;
			while(--time);
}
void delay_ms(uint16_t time)
{
			while(time--)
				delay_us(1000);
}

void Read_Temp(void)
{
		uint8_t WriteBuffer[2] = {0x33,0x00};
		uint8_t ReadBuffer[6] = {0};
		uint32_t wendu=0x00000000,shidu=0x00000000;
		double   wendu1,shidu1,wendu2,shidu2;
		char string[10];
		I2C1_Write_NBytes(0x70, 0xac, 2, WriteBuffer);
		delay_ms(2000);
		I2C1_Read_NBytes(0x71,  6, ReadBuffer);	
		
		shidu |= ReadBuffer[1];
		shidu = shidu << 8;
		shidu |= ReadBuffer[2];
		shidu = shidu << 4;
		shidu |= ReadBuffer[3] >> 4;
		
		wendu |= ReadBuffer[3] & 0x0f;
		wendu = wendu << 8;
		wendu |= ReadBuffer[4];
		wendu = wendu << 8;
		wendu |= ReadBuffer[5] ;
		
		shidu1 = (double)shidu;
		wendu1 = (double)wendu;
		
		shidu2 =   shidu1 *1.00 /1024 /1024 *100;
		wendu2 =  wendu1 *1.00/1024 /1024 *200- 50;
		
		sprintf(string,"%.2lf",shidu2);
		P8x16Str(55,1,(uint8_t *)string);
		sprintf(string,"%.2lf",wendu2);
		P8x16Str(55,3,(uint8_t *)string);
		
		shidu = 0;wendu = 0;
}

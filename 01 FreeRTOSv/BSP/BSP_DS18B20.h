#ifndef __BSP_DS18B20_H__
#define __BSP_DS18B20_H__

#include "stm32f10x.h"

extern volatile float result;

#define DS18B20_IO_IN()  {GPIOE->CRL&=0XFFF0FFFF;GPIOE->CRL|=8<<16;} 
#define DS18B20_IO_OUT()  {GPIOE->CRL&=0XFFF0FFFF;GPIOE->CRL|=3<<16;} 


#define DS18B20_DQ_OUT_1()  {GPIO_SetBits(GPIOE,GPIO_Pin_4);} //设置高电平
#define DS18B20_DQ_OUT_0()  {GPIO_ResetBits(GPIOE,GPIO_Pin_4);}  //设置低电平

#define DS18B20_DQ_IN()  {GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_4);}



void BSP_DS18B20_Rst(void);

uint8_t BSP_DS18B20_Check(void);

uint8_t BSP_DS18B20_Init(void);

void BSP_DS18B20_Write_Byte(uint8_t data);

uint8_t BSP_DS18B20_Read_Bit(void);

uint8_t BSP_DS18B20_Read_Byte(void);

void BSP_DS18B20_Start(void);

short BSP_DS18B20_Get_Temp(void);

void SenSorData_Output(void);

#endif

#ifndef __BSP_DS18B20_H__
#define __BSP_DS18B20_H__

#include <stdio.h>
#include "stm32f10x.h"
#include "BSP_Delay.h"

extern volatile float result;
//IO方向设置
#define DS18B20_IO_IN()  {GPIOE->CRL&=0XFFF0FFFF;GPIOE->CRL|=8<<16;}
#define DS18B20_IO_OUT() {GPIOE->CRL&=0XFFF0FFFF;GPIOE->CRL|=3<<16;}

//IO操作函数											   
#define	DS18B20_DQ_OUT PEout(4) //数据端口	PE4 
#define	DS18B20_DQ_IN  PEin(4)  //数据端口	PE                   4 

uint8_t BSP_DS18B20_Init(void);//初始化DS18B20

void BSP_DS18B20_Rst(void);

uint8_t BSP_DS18B20_Check(void);

uint8_t BSP_DS18B20_Read_Bit(void);	

uint8_t BSP_DS18B20_Read_Byte(void);

void BSP_DS18B20_Write_Byte(uint8_t data);

void BSP_DS18B20_Start(void);

short BSP_DS18B20_Get_Temp(void);

void SenSorData_Output(void);

#endif

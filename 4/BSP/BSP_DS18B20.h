#ifndef _BSP_DS18B20_H
#define _BSP_DS18B20_H

#include "stdio.h"
#include "stm32f10x.h"
#include "BSP_Delay.h"
#include "BSP_DebugUART.h"
#include "BSP_Buzzer.h"

extern volatile float result;

//IO方向设置
#define DS18B20_IO_IN()  {GPIOE->CRL&=0XFFF0FFFF;GPIOE->CRL|=8<<16;}
#define DS18B20_IO_OUT() {GPIOE->CRL&=0XFFF0FFFF;GPIOE->CRL|=3<<16;}

/* //IO操作函数											   
#define	DS18B20_DQ_OUT PEout(4) //数据端口	PE4 
#define	DS18B20_DQ_IN  PEin(4)  //数据端口	PE4 */

#define DS18B20_DQ_OUT_1() {GPIO_SetBits(GPIOE,GPIO_Pin_4);} ///设置成高电平
#define DS18B20_DQ_OUT_0() {GPIO_ResetBits(GPIOE,GPIO_Pin_4);}





//0.初始化
uint8_t BSP_DS18B20_Init(void);

//1.复位
void BSP_DS18B20_Rst(void);

//2.检测设备是否存在功能的实现
uint8_t BSP_DS18B20_Check(void);

//3.写一个字节到DS18B20形参：要写入的字节。
void BSP_DS18B20_Write_Byte(uint8_t data);

//4.读时序
uint8_t BSP_DS18B20_Read_Bit(void);

// 从DS18B20读取一个字节,返回值：读到的数据
uint8_t BSP_DS18B20_Read_Byte(void);

// 5.开始温度转换
void BSP_DS18B20_Start(void);

// 6.DS18B20的典型温度读取
short BSP_DS18B20_Get_Temp(void);

//7.温度结果转换 和 阈值预警蜂鸣器
void SenSorData_Output(void);


#endif

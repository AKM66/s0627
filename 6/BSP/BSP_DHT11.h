#ifndef __BSP_DHT11_H
#define __BSP_DHT11_H

#include "stm32f10x.h"
#include "BSP_Delay.h"
#include "math.h"
#include "stdio.h"

#define DHT11_RCC_CLK		(RCC_APB2Periph_GPIOA)
#define DHT11_PIN				GPIO_Pin_1
#define DHT11_PORT			GPIOA


//IO�������� 
#define DHT11_IO_IN()  {DHT11_PORT->CRL&=0XFFFFFF0F;DHT11_PORT->CRL|=8<<4;}
#define DHT11_IO_OUT() {DHT11_PORT->CRL&=0XFFFFFF0F;DHT11_PORT->CRL|=3<<4;}
////IO��������											   
#define	DHT11_OUT PAout(1) //���ݶ˿�	PA1
#define	DHT11_IN  PAin(1)  //���ݶ˿�	PA1

void BSP_DHT11_Init(void);		//��ʼ��DHT11
void BSP_DHT11_Reset(void);		//��λDHT11

uint8_t BSP_DHT11_Read_Data(uint8_t *temp_int,uint8_t *humi_int,uint8_t *temp_float,uint8_t *humi_float);  	//��ȡ��ʪ��
uint8_t BSP_DHT11_Read_Byte(void);	//����һ���ֽ�
uint8_t BSP_DHT11_Read_Bit(void);	//����һ��λ
uint16_t BSP_DHT11_CheckDevice(void);		//����Ƿ����DHT11
float BSP_DHT11_CalcuDewPoint(float t, float h);

#endif // __BSP_DHT11_H

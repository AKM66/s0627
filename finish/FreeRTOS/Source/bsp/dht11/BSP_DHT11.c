/*
***************************************************************************
*    模块：BSP_DHT11 
*    描述：板级 DHT11 功能模块驱动
*    作者：Shao
*    时间：2018.06.12
*    版本：UP-IOT 2.0.0
***************************************************************************
*/
#include "BSP_DHT11.h"

/*
*******************************************************************************
*	函 数 名: _DelayUs
*	功能说明: 非精准us级别延时函数
*	形    参: uint16_t xUs
*	返 回 值: 无
*******************************************************************************
*/
static void _DelayUs(uint16_t xUs)
{
	while(xUs != 0){
		xUs--;
	}
}

/*
*******************************************************************************
*	函 数 名: _DelayMs
*	功能说明: 非精准ms级别延时函数
*	形    参: uint16_t count
*	返 回 值: 无
*******************************************************************************
*/
/* 非精确延时函数 */
static void _DelayMs(uint16_t xMs)
{
	volatile uint32_t Count=8000;
	while(xMs--)
	{
		Count=8000;
		while(Count--);
	}
}

/*
***************************************************************************
*	函 数 名: BSP_DHT11_Init
*	功能说明: DHT11初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;        
	RCC_APB2PeriphClockCmd(DHT11_RCC_CLK,ENABLE);
			
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);

	BSP_DHT11_Reset();        //复位通讯
}

/*
***************************************************************************
*	函 数 名: BSP_DHT11_Reset
*	功能说明: DHT11复位函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_DHT11_Reset(void)
{
	DHT11_IO_OUT(); 			//设置输出模式
  DHT11_OUT=0; 					//拉低DQ		
//  _DelayMs(18);					//拉低至少18ms
	BSP_Delay_ms(20);
	DHT11_OUT=1; 					//DQ=1 
//	_DelayUs(320);				//主机拉高20~40us     35
	BSP_Delay_us(35);
}

/*
***************************************************************************
*	函 数 名: BSP_DHT11_CheckDevice
*	功能说明: 检测DHT11设备是否存在函数
*	形    参: 无
*	返 回 值: 返回0代表检测到DHT11设备，返回1代表没有检测到DHT11设备
***************************************************************************
*/
uint16_t BSP_DHT11_CheckDevice(void){
	uint8_t delay_cnt = 0;
	DHT11_IO_IN();													//设置输入模式
	while(DHT11_IN&&delay_cnt<100)					//DHT11会拉低40~80us
	{
		delay_cnt++;
		BSP_Delay_us(10);
//		_DelayUs(80);
	};	 
	if(delay_cnt >= 100){										//超时返回1
		return 1;
	}
	else {
		delay_cnt = 0;
	}
  while((!DHT11_IN) && (delay_cnt < 100))	//DHT11拉低后会再次拉高40~80us
	{
		delay_cnt++;
		BSP_Delay_us(10);
//		_DelayUs(80);
	};
	if(delay_cnt >= 100){
		return 1;
	}		
	return 0;
}


/*
***************************************************************************
*	函 数 名: DHT11_Read_Bit
*	功能说明: 读取1Bit函数
*	形    参: 无
*	返 回 值: 返回读到的数据是0还是1
***************************************************************************
*/
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t delay_cnt=0;
	while(DHT11_IN&&delay_cnt<100)	//等待变为低电平
	{
		delay_cnt++;
		BSP_Delay_us(10);
//		_DelayUs(80);
	}
	delay_cnt = 0;
	while(!DHT11_IN&&delay_cnt<100)	//等待变高电平
	{
		delay_cnt++;
		BSP_Delay_us(10);
//		_DelayUs(80);
	}
		BSP_Delay_us(35);
//	_DelayUs(320);									//等待40us	数字1信号的延时比数字0信号的延时长大约40us.
	if(DHT11_IN){										//判断引脚状态
		return 1;
	}
	else 
		return 0;	
}

/*
***************************************************************************
*	函 数 名: DHT11_Read_Byte
*	功能说明: 读取1字节函数
*	形    参: 无
*	返 回 值: 返回读到的数据
***************************************************************************
*/
uint8_t DHT11_Read_Byte(void)    
{        
  uint8_t i;
  uint8_t data=0;
	
	for (i=0;i<8;i++) 
	{
   		data <<= 1; 
	    data |= DHT11_Read_Bit();
    }						    
    return data;
}

/*
***************************************************************************
*	函 数 名: BSP_DHT11_Read_Data
*	功能说明: 读取温湿度传感器数据
*	形    参: uint8_t *temp_int:存放温度值整数部分
						uint8_t *humi_int:存放湿度值整数部分
						uint8_t *temp_float:存放温度值小数部分
						uint8_t *humi_float:存放湿度值小数部分
*	返 回 值: 返回读到的数据
***************************************************************************
*/
uint8_t BSP_DHT11_Read_Data(uint8_t *temp_int,uint8_t *humi_int,uint8_t *temp_float,uint8_t *humi_float)    
{        
 	uint8_t buf[5];
	uint8_t i;
	BSP_DHT11_Reset();
	if(BSP_DHT11_CheckDevice() == 0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi_int = buf[0];
			*humi_float = buf[1];
			*temp_int = buf[2];
			*temp_float = buf[3];
		}
	}
	else 
		return 1;
	return 0;	    
}

/*
***************************************************************************
*	函 数 名: BSP_DHT11_CalcuDewPoint
*	功能说明: 计算露点
*	形    参: h-实际的湿度；t-实际的温度 
*	返 回 值: dew_point-露点
***************************************************************************
*/
float BSP_DHT11_CalcuDewPoint(float t, float h)
{
	float logEx, dew_point;

	logEx = 0.66077 + 7.5 * t / (237.3 + t) + (log10(h) - 2);
	dew_point = ((0.66077 - logEx) * 237.3) / (logEx - 8.16077);

	return dew_point; 
}

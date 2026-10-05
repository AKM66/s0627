#include "BSP_DigitalTube.h"
#include "./usart/bsp_usart1.h"
uint8_t smgduan[11]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uint16_t smgwei[6]={0x7fff,0xbfff,0xdfff,0xefff,0xf7ff,0xfbff};

/****************************************************************
* 函 数 名         : BSP_DigitalTube_Init
* 函数功能		   : 数码管显示初始化
* 输    入         : 无
* 输    出         : 无
****************************************************************/
void BSP_DigitalTube_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	//打开GPIOE和GPIOG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG , ENABLE); 
	
	//配置GPIOE为输出推挽模式, 为避免后续中断产生误动作, 只初始化高8位	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|
	GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	//配置GPIOG为输出推挽模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 
	GPIO_Init(GPIOG, &GPIO_InitStructure); 

}

void Show_Temperature(float data)
{
	int cnt,i;
//	while(1)
//	{
	int temp = data * 100;	
	int chushu = 1000;
		for(cnt = 0; cnt < 4; cnt++)
		{
			GPIO_Write(GPIOE,smgwei[2+cnt]);
			if(cnt == 1)
			{
				GPIO_Write(GPIOG,smgduan[temp/chushu]&0xff7f);
			}
			else
			{
				GPIO_Write(GPIOG,smgduan[temp/chushu]);
			}
			temp = temp%chushu;
			chushu = chushu / 10;
			for(i=0;i<19999;i++);
		}
//	}
}


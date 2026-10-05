#include "beepkey.h"

/**************************************************************************
* 函 数 名         : delay
* 函数功能		   : 简易延时函数
* 输    入         : 无
* 输    出         : 无
**************************************************************************/
void delay(u32 i)
{
	while(i--) ;
}

/*************************************************************************
* 函 数 名         : BeepInit
* 函数功能		   : 蜂鸣器端口初始化函数	   通过改变频率控制声音变化
* 输    入         : 无
* 输    出         : 无
**************************************************************************/
void BeepInit()	  //端口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;	//声明一个结构体变量，用来初始化GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   /* 开启GPIO时钟 */

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;		//选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		  //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOC,&GPIO_InitStructure); 	 /* 初始化GPIO */
}

/************************************************************************
* 函 数 名         : KeyInit
* 函数功能		   : 按键端口初始化函数	   通过改变频率控制声音变化
* 输    入         : 无
* 输    出         : 无
*************************************************************************/
void KeyInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//声明一个结构体变量，用来初始化GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);   /* 开启GPIO时钟 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置上拉输入模式
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
}


/*************************************************************
* 函 数 名         : sound1
* 函数功能		   : 蜂鸣器报警函数
* 输    入         : 无
* 输    出         : 无
**************************************************************/
void sound1()  //救护车报警
{
	u32 i=5000;
	while(i--)			   //产生一段时间的PWM波，使蜂鸣器发声
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_8);		 //IO口输出高电平
		delay(i);
		GPIO_ResetBits(GPIOC,GPIO_Pin_8);	  //IO口输出低电平
		delay(i--);
	}	
}

/*************************************************************
* 函 数 名         : sound2
* 函数功能		   : 蜂鸣器报警函数	  通过改变频率控制声音变化
* 输    入         : 无
* 输    出         : 无
**************************************************************/
void sound2()  //电动车报警
{
	u32 i=1000;
	while(i--)					//产生一段时间的PWM波，使蜂鸣器发声
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_8);	   //IO口输出高电平
		delay(i);
		GPIO_ResetBits(GPIOC,GPIO_Pin_8);	//IO口输出低电平
		delay(i--);
	}	
}

/************************************************************************
* 函 数 名         : BeepKey
* 函数功能		   : 检测按键   控制蜂鸣器发不同报警声
* 输    入         : 无
* 输    出         : 无
*************************************************************************/
void BeepKey()
{
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)==0)	  //判断按键PE0是否按下
			{
				delay_ms(10); //消抖处理
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)==0)	 //再次判断按键PE0是否按下
				{
					sound1()  ;				
				}
				while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)==0); //等待按键松开
			}
			
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0)	  //判断按键PE1是否按下
			{
				delay_ms(10); //消抖处理
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0)	 //再次判断按键PE1是否按下
				{
					sound2();				
				}
				while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0); //等待按键松开
			}
	}
}


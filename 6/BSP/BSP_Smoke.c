/*
***************************************************************************
*    模块：BSP_SNOW
*    描述：板级 SNOW 功能模块驱动
*    作者：Huo
*    时间：2017.09.01
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_Smoke.h"

/* 内部函数声明 */
static void BSP_SMOKE_GPIO_Init(void);

/*
***************************************************************************
*	函 数 名: BSP_SMOKE_Init
*	功能说明: 板级 SNOW 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_SMOKE_Init(void)
{
	BSP_SMOKE_GPIO_Init();
}

/*
***************************************************************************
*	函 数 名: BSP_SMOKE_GPIO_Init
*	功能说明: 板级 SMOKE GPIO 内部初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_SMOKE_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_SMOKE, ENABLE);//使能对应时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SMOKE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 //浮空输入
	GPIO_Init(GPIO_PORT_SMOKE, &GPIO_InitStructure);
}


bool BSP_SMOKE_Scan(void)
{
	uint8_t SMOKE_State=false;
	
	if(GPIO_ReadInputDataBit(GPIO_PORT_SMOKE,GPIO_PIN_SMOKE)){
		SMOKE_State = true;
	}
	else {
		SMOKE_State = false;
	}
	return SMOKE_State;
}

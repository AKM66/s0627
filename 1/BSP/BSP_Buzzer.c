/*
***************************************************************************
*    模块：BSP_LEDBuzzer 
*    描述：板级 LEDBuzzer 功能模块驱动
*    作者：Huo
*    时间：2017.09.01
*    版本：UP-IOT 1.0.0
***************************************************************************
*/
#include "BSP_Buzzer.h"

/* 内部函数声明 */
static void BSP_Buzzer_GPIO_Init(void);
/*
***************************************************************************
*	函 数 名: BSP_Buzzer_Init
*	功能说明: 板级 Buzzer 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_Buzzer_Init(void)
{
	BSP_Buzzer_GPIO_Init();
}

/*
***************************************************************************
*	函 数 名: BSP_Buzzer_GPIO_Init
*	功能说明: 板级 Buzzer GPIO 内部初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_Buzzer_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_Buzzer, ENABLE);//使能对应时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_Buzzer;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_PORT_Buzzer, &GPIO_InitStructure);
	
	BSP_Buzzer_Off();
}

/*
***************************************************************************
*	函 数 名: BSP_Buzzer_On
*	功能说明: 板级 Buzzer发声 功能函数
*	形    参: Buzzer
*	返 回 值: 无
***************************************************************************
*/
void BSP_Buzzer_On(void)
{
	// GPIO_PORT_Buzzer->BSRR = GPIO_PIN_Buzzer;
    uint32_t duration_ms = 1000;
    while (duration_ms--)  // 持续1秒
    {
        GPIO_PORT_Buzzer->BSRR = GPIO_PIN_Buzzer;  // 拉高（80%时间）
        delay_us(800);  // 高电平持续时间（占空比80%）
        
        GPIO_PORT_Buzzer->BRR = GPIO_PIN_Buzzer;   // 拉低（20%时间）
        delay_us(200);  // 低电平持续时间
		delay_ms(1);
    }
}


/*
***************************************************************************
*	函 数 名: BSP_Buzzer_Off
*	功能说明: 板级 Buzzer息声 功能函数
*	形    参: Buzzer
*	返 回 值: 无
***************************************************************************
*/
void BSP_Buzzer_Off(void)
{
	GPIO_PORT_Buzzer->BRR = GPIO_PIN_Buzzer;
}

/*
***************************************************************************
*	函 数 名: BSP_Buzzer_Toggle
*	功能说明: 板级 Buzzer 状态翻转功能函数
*	形    参: Buzzer
*	返 回 值: 无
***************************************************************************
*/
void BSP_Buzzer_Toggle(void)
{
	GPIO_PORT_Buzzer->ODR ^= GPIO_PIN_Buzzer;
}



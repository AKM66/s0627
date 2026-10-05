/*
***************************************************************************
*    模块：BSP_LEDBuzzer 
*    描述：板级 LEDBuzzer 功能模块驱动
		   LED-PG15 高电平点亮，Buzzer-PF11 高电平发声
*    作者：Huo
*    时间：2017.09.01
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_LEDBuzzer.h"

unsigned char led_state[10] ="ledoff";
unsigned char buzzer_state[10] ="buzzeroff";
unsigned char alarmlamp_state[15] ="alarmlampoff";

uint16_t GPIO_Pins[] = {
	GPIO_PIN_LED0,
	GPIO_PIN_LED1,
	GPIO_PIN_LED2,
	GPIO_PIN_LED3,
	GPIO_PIN_LED4,
	GPIO_PIN_LED5,
	GPIO_PIN_LED6,
	GPIO_PIN_LED7
};
/* 内部函数声明 */
static void BSP_LEDBuzzer_GPIO_Init(void);

/*
***************************************************************************
*	函 数 名: BSP_LEDBuzzer_Init
*	功能说明: 板级 LEDBuzzer 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_LEDBuzzer_Init(void)
{
	BSP_LEDBuzzer_GPIO_Init();
}

/*
***************************************************************************
*	函 数 名: BSP_LEDBuzzer_GPIO_Init
*	功能说明: 板级 LEDBuzzer GPIO 内部初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_LEDBuzzer_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_LED | RCC_Buzzer, ENABLE);//使能对应时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED0|GPIO_PIN_LED1|GPIO_PIN_LED2|GPIO_PIN_LED3|\
	GPIO_PIN_LED4|GPIO_PIN_LED5|GPIO_PIN_LED6|GPIO_PIN_LED7;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //推挽输出
	GPIO_Init(GPIO_PORT_LED, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_Buzzer;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_PORT_Buzzer, &GPIO_InitStructure);
	
	BSP_LED_Off(8);
}

/*
***************************************************************************
*	函 数 名: BSP_LED_On
*	功能说明: 板级 LED点亮 功能函数
*	形    参: number
*	返 回 值: 无
***************************************************************************
*/
void BSP_LED_On(uint8_t number)
{
	int i;
		for(i = 0; i< 8;i++)
	{
		GPIO_SetBits(GPIO_PORT_LED, GPIO_Pins[i]);
	}
	for(i = 0; i< number;i++)
	{
		GPIO_ResetBits(GPIO_PORT_LED, GPIO_Pins[i]);
	}	
}
/*
***************************************************************************
*	函 数 名: BSP_LEDBuzzer_Off
*	功能说明: 板级 LED熄灭、Buzzer息声 功能函数
*	形    参: LEDBuzzer,number
*	返 回 值: 无
***************************************************************************
*/
void BSP_LED_Off(uint8_t number)
{
	int i;

	for(i = 0; i< number;i++)
	{
		GPIO_SetBits(GPIO_PORT_LED, GPIO_Pins[i]);
	}
}


/*
***************************************************************************
*	函 数 名: BSP_Buzzer_On
*	功能说明: 板级 Buzzer发声 功能函数
*	形    参: void
*	返 回 值: 无
***************************************************************************
*/
void BSP_Buzzer_On(void)
{
	GPIO_ResetBits(GPIO_PORT_Buzzer,GPIO_PIN_Buzzer);
}

/*
***************************************************************************
*	函 数 名: BSP_Buzzer_Off
*	功能说明: 板级 Buzzer发声 功能函数
*	形    参: void
*	返 回 值: 无
***************************************************************************
*/
void BSP_Buzzer_Off(void)
{
	GPIO_SetBits(GPIO_PORT_Buzzer,GPIO_PIN_Buzzer);
}


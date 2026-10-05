//驱动USART3 USART3用于WIFI模块的控制

#include "BSP_UART.h"

/* 内部函数声明 */
static void BSP_UART_GPIO_Init(void);
static void BSP_UART_USART_Init(uint32_t BaudRate);
static void BSP_UART_NVIC_Init(void);

/*
***************************************************************************
*	函 数 名: BSP_USART3_Init
*	功能说明: 板载 USART3 初始化函数
*	形    参: BaudRate 波特率
*	返 回 值: 无
***************************************************************************
*/
void BSP_USART3_Init(uint32_t BaudRate)
{
	BSP_UART_USART_Init(BaudRate);
	BSP_UART_GPIO_Init();
	BSP_UART_NVIC_Init();
}

/*
***************************************************************************
*	函 数 名: BSP_UART_GPIO_Init
*	功能说明: 板载 BSP_UART GPIO 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_UART_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟
	
	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 		//PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//初始化GPIOB.10

	//USART3_RX	  GPIOB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//初始化GPIOB.11 
}

/*
***************************************************************************
*	函 数 名: BSP_UART_USART_Init
*	功能说明: 板载 UART USART 初始化函数
*	形    参: BaudRate 波特率
*	返 回 值: 无
***************************************************************************
*/
static void BSP_UART_USART_Init(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;	//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = \
	                    USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = \
	                    USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); 	//初始化串口1
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    //使能串口1 
}

/*
***************************************************************************
*	函 数 名: BSP_UART_NVIC_Init
*	功能说明: 板载 UART NVIC 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_UART_NVIC_Init(void)
{
 	NVIC_InitTypeDef NVIC_InitStructure;

	//USART3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	/* 抢占优先级设置，优先级分组为 4 的情况下，抢占优先级可设置范围 0-15 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 12;
	/* 子优先级设置，优先级分组为 4 的情况下，子优先级无效，取数值 0 即可 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}


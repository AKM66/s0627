/**
  ******************************************************************************
  * @file    bsp_usart1.c
	* @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口1配置,配合printf
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "./usart/bsp_usart1.h"


char Usart1_Rx_Buf[USART1_RX_BUFF_SIZE];
uint8_t RxCounter1 = 0;

/**
 * @brief  USART GPIO 配置初始化
 * @param  usart_baudRate 波特率 USART_BAUDRATE_115200/USART_BAUDRATE_9600
 * @retval None
 */
 
static void DebugUART_GPIO_Init(void);
static void DebugUART_USART_Init(uint32_t usart_baudRate);
static void DebugUART_NVIC_Init(void);
 
static void DebugUART_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/** 打开串口GPIO的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

/** 将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/** 将USART Rx的GPIO配置为浮空输入模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}
static void DebugUART_USART_Init(uint32_t usart_baudRate)
{
	USART_InitTypeDef USART_InitStructure;
	 /** 打开串口外设的时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/** 配置串口的工作参数 */
	/** 配置波特率 */
	USART_InitStructure.USART_BaudRate = usart_baudRate;
	/** 配置 针数据字长 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/** 配置停止位 */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/** 配置校验位 */
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	/** 配置硬件流控制 */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/** 配置工作模式，收发一起 */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/** 完成串口的初始化配置 */
	USART_Init(USART1, &USART_InitStructure);
	
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	/** 使能串口 */
	USART_Cmd(USART1, ENABLE);
}

static void DebugUART_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/** 使能串口中断接受 */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BSP_USART1_RX_NVIC_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
 
void USART1_Init(uint32_t usart_baudRate)
{
  DebugUART_GPIO_Init();
	DebugUART_USART_Init(usart_baudRate);
	DebugUART_NVIC_Init();
}

/**
  * @brief  串口发送数据
  * @param  pData 数据指针
  * @param  len 数据长度
  */
void USART1_Send(const uint8_t* const pData, uint32_t len)
{
    int i;
    for( i = 0; i<len ; i++)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, pData[i]);
    }
}
/**
  * @brief  串口发送字符
  * @param  ch 要发送的字符
  * @retval 发送的字符
  */
int USART1_SendChar(int ch)
{
    USART_SendData(USART1, (uint8_t) ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return ch;
}

/**
  * @brief  串口接受字符
  * @retval 接受的字符
  */
int USART1_GetChar(void)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(USART1);
}

/*********************************************END OF FILE**********************/


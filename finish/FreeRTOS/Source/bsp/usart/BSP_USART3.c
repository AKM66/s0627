#include "BSP_USART3.h"
#include "./usart/bsp_usart1.h"
#include "main.h"
#include <stdarg.h>

uint8_t Usart3_Rx_Buf[USART3_RX_BUFF_SIZE];

static void USART3_GPIO_Init(void);
static void USART3_NVIC_Init(void);
static void USART3_USART_Init(uint32_t usart_baudRate);

static char * itoa( int value, char *string, int radix );

static void USART3_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	/* Configure USART3 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USART3 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void USART3_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void USART3_USART_Init(uint32_t usart_baudRate)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	USART_InitStructure.USART_BaudRate = usart_baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);

	USART_ClearFlag(USART3, USART_FLAG_TC);		//发送完成标志位
	//发送完成中断
	USART_ITConfig(USART3, USART_IT_TC, DISABLE);
	//接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
	//空闲总线中断
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	//采用DMA方式接收
  USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);   //串口收DMA配置
	
	USART_Cmd(USART3, ENABLE);
}

static void USART3_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

  /*dma配置*/

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//启动DMA时钟

  DMA_DeInit(DMA1_Channel3); //DMA1通道3配置，将 DMA1的通道 13寄存器重设为缺省值

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);					//外设地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart3_Rx_Buf;					//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//dma传输方向单向
  DMA_InitStructure.DMA_BufferSize = USART3_RX_BUFF_SIZE; 								//设置DMA在传输时缓冲区的长度
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//设置DMA的外设递增模式，一个外设，地址不自增
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//设置DMA的内存递增模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据字长
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//内存数据字长
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;												//设置DMA的传输模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;							//设置DMA的优先级别
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 												//设置DMA的2个memory中的变量互相访问
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);

  DMA_Cmd(DMA1_Channel3, ENABLE);//使能通道3
}

void USART3_Init(uint32_t usart_baudRate)
{
	USART3_GPIO_Init();
	USART3_USART_Init(usart_baudRate);
  USART3_NVIC_Init(); 
	USART3_DMA_Init();	
}


/**
  * @brief  串口3发送数据
  * @param  pData 数据指针
  * @param  len 数据长度
  */
void USART3_Send(const char* const pData, uint32_t len)
{
    int i;
    for( i = 0; i<len ; i++)
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  //发送完成标志位
        USART_SendData(USART3, pData[i]);
    }
}
/**
  * @brief  串口3发送数据
  * @param  pData 数据指针
  * @param  len 数据长度
  */
void USART3_SendData(const uint8_t* const pData, uint32_t len)
{
    int i;
    for( i = 0; i<len ; i++)
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);   //发送完成标志位
        USART_SendData(USART3, pData[i]);
    }
}
/**
  * @brief  串口发送字符
  * @param  ch 要发送的字符
  * @retval 发送的字符
  */
int USART3_SendChar(int ch)
{
    USART_SendData(USART3, (uint8_t) ch);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);  //发送数据寄存器空标志位

    return ch;
}

/*
***************************************************************************
*	函 数 名: USART_printf
*	功能说明: 格式化输出，类似于C库中的printf，但这里没有用到C库
*	形    参: -USARTx 串口通道，这里只用到了串口3，即USART3
*             -Data   要发送到串口的内容的指针
*             -...    其他参数
*	返 回 值: 无
***************************************************************************
*/
void USART_printf( USART_TypeDef * USARTx, char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // 判断是否到达字符串结束符
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

				case 'n':							          //换行符
				USART_SendData(USARTx, 0x0a);	
				Data ++;
				break;

				default:
				Data ++;
				break;
			}			 
		}
		
		else if ( * Data == '%')
		{									  
			switch ( *++Data )
			{				
				case 's':										  //字符串
				s = va_arg(ap, const char *);
				
				for ( ; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;

				case 'd':			
					//十进制
				d = va_arg(ap, int);
				
				itoa(d, buf, 10);
				
				for (s = buf; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;
				
				default:
				Data++;
				
				break;
			}		 
		}
		
		else USART_SendData(USARTx, *Data++);
		
		while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );
	}
}

/*
***************************************************************************
*	函 数 名: itoa
*	功能说明: 将整形数据转换成字符串
*	形    参: -radix =10 表示10进制，其他结果为0
*             -value 要转换的整形数
*             -buf 转换后的字符串
*             -radix = 10
*	返 回 值: 无
***************************************************************************
*/
static char * itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;
	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;
}

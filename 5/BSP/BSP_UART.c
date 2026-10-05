#include <stdarg.h>
#include "BSP_UART.h"

uint8_t Usart3_Rx_Buf[USART3_RX_BUFF_SIZE];

/* �ڲ��������� */
static void BSP_UART_GPIO_Init(void);
static void BSP_UART_USART_Init(uint32_t BaudRate);
static void BSP_UART_NVIC_Init(void);
static char * itoa( int value, char * string, int radix );
static void BSP_USART_DMA_Init(void);

/*
***************************************************************************
*	�� �� ��: BSP_UART_Init
*	����˵��: ���� UART ��ʼ������
*	��    ��: BaudRate ������
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_USART_Init(uint32_t BaudRate)
{
	BSP_UART_USART_Init(BaudRate);
	BSP_UART_GPIO_Init();
	BSP_UART_NVIC_Init();
	BSP_USART_DMA_Init();
}

/*
***************************************************************************
*	�� �� ��: BSP_UART_GPIO_Init
*	����˵��: ���� UART GPIO ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_UART_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��

	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.2

	//USART3_RX	  GPIOB.11��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.3  
}

/*
***************************************************************************
*	�� �� ��: BSP_UART_USART_Init
*	����˵��: ���� UART USART ��ʼ������
*	��    ��: BaudRate ������
*	�� �� ֵ: ��
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

	USART_Init(USART3, &USART_InitStructure); 	//初始化串口3
//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	
	USART_ClearFlag(USART3, USART_FLAG_TC);		//发送完成标志位
	//发送完成中断
	USART_ITConfig(USART3, USART_IT_TC, DISABLE);
	//接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
	//空闲总线中断
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	//采用DMA方式接收
  USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);   //串口收DMA配置
	
	USART_Cmd(USART3, ENABLE);                    //使能串口3 
}

/*
***************************************************************************
*	�� �� ��: BSP_UART_NVIC_Init
*	����˵��: ���� UART NVIC ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_UART_NVIC_Init(void)
{
 	NVIC_InitTypeDef NVIC_InitStructure;

	//USART1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	/* ��ռ���ȼ����ã����ȼ�����Ϊ 4 ������£���ռ���ȼ������÷�Χ 0-15 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 13;
	/* �����ȼ����ã����ȼ�����Ϊ 4 ������£������ȼ���Ч��ȡ��ֵ 0 ���� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

/*
***************************************************************************
*	�� �� ��: USART_printf
*	����˵��: ��ʽ�������������C���е�printf��������û���õ�C��
*	��    ��: -USARTx ����ͨ��������ֻ�õ��˴���3����USART3
*             -Data   Ҫ���͵����ڵ����ݵ�ָ��
*             -...    ��������
*	�� �� ֵ: ��
***************************************************************************
*/
void USART_printf( USART_TypeDef * USARTx, char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

				case 'n':							          //���з�
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
				case 's':										  //�ַ���
				s = va_arg(ap, const char *);
				
				for ( ; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;

				case 'd':			
					//ʮ����
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
*	�� �� ��: itoa
*	����˵��: ����������ת�����ַ���
*	��    ��: -radix =10 ��ʾ10���ƣ��������Ϊ0
*             -value Ҫת����������
*             -buf ת������ַ���
*             -radix = 10
*	�� �� ֵ: ��
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

static void BSP_USART_DMA_Init(void)
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





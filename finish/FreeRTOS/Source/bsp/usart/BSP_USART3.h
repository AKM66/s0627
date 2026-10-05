#ifndef __BSP_USART3_H_
#define	__BSP_USART3_H_

#include "stm32f10x.h"

#define USART3_RX_BUFF_SIZE   (256)              /*串口3接收数据缓冲区*/	

extern uint8_t Usart3_Rx_Buf[USART3_RX_BUFF_SIZE];

typedef void (*usart_recv_callback)(uint8_t ch);

typedef void (*dma_recv_callback);


void USART3_Init(uint32_t usart_baudRate);

void USART3_Send(const char* const pData, uint32_t len);

void USART3_SendData(const uint8_t* const pData, uint32_t len);

void USART_printf( USART_TypeDef * USARTx, char * Data, ... );

#endif

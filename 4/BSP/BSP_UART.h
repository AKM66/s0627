#ifndef __BSP_UART_H
#define __BSP_UART_H

#include <stdio.h>
#include "stm32f10x.h"
#include <string.h>

#define USART3_RX_BUFF_SIZE   (256)  

extern uint8_t Usart3_Rx_Buf[USART3_RX_BUFF_SIZE];

void BSP_USART_Init(uint32_t BaudRate);
void USART_printf( USART_TypeDef * USARTx, char * Data, ... );

#endif

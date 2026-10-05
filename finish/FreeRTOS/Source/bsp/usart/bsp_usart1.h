#ifndef __BSP_USART1_H_
#define	__BSP_USART1_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"
#include "main.h"

#define USART1_RX_BUFF_SIZE   (256)              /*< 串口1接收数据缓冲区 >*/

extern uint8_t RxCounter1;

extern char Usart1_Rx_Buf[USART1_RX_BUFF_SIZE];

void USART1_Init(uint32_t usart_baudRate);

void USART1_Send(const uint8_t* const pData, uint32_t len);

int USART1_SendChar(int ch);

int USART1_GetChar(void);
	

#ifdef __cplusplus
}
#endif

#endif 


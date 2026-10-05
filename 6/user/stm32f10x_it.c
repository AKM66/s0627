/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "BSP_DigitalTube.h"
#include "BSP_DS18B20.h"
#include "BSP_UART.h"
#include "BSP_ESP8266.h"
#include "Queue.h"
#include "UserTask.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */

/* void SVC_Handler(void)
{
} */

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */

/* void PendSV_Handler(void)
{
} */

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

/* void SysTick_Handler(void)
{
} */

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/*
***************************************************************************
*	        : USART1_IRQHandler
*	    ˵  : STM32 USART1  жϷ     
*	        :   
*	      ֵ:   
***************************************************************************
*/
void USART1_IRQHandler(void)
{
	uint16_t Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART1); 
		QueueMemDataInsert((uint8_t)Res);	
	}
}

void TIM2_IRQHandler(void)
{
  static int cnt = 0;
  if (TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)//检查TIM2更新中断
  {
    if (cnt == 5)
    {
      Show_Temperature(result);
      cnt = 0;
    }
    cnt++;
  }
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断标志
}

extern volatile uint8_t ucTcpClosedFlag;  //Test_ESP8266.c
/*
***************************************************************************
*	        : USART2_IRQHandler
*	    ˵  : STM32 USART2  жϷ     
*	        :   
*	      ֵ:   
***************************************************************************
*/
/* void USART3_IRQHandler(void)
{
	uint8_t ucCh;
	
	if ( USART_GetITStatus ( USART3, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( USART3 );
		if ( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )   
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ucCh;
	}
	 	 
	if ( USART_GetITStatus( USART3, USART_IT_IDLE ) == SET )
	{
		strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
		ucCh = USART_ReceiveData( USART3 );

		ucTcpClosedFlag = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
	}	
} */

void USART3_IRQHandler(void)
{

    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) 		//空闲总线中断
    {
        DMA_Cmd(DMA1_Channel3,DISABLE);
        USART3->SR;						//先读SR
        USART3->DR;						 //读SR后读DR 清USART_IT_IDLE标志
			
        DMA_SetCurrDataCounter(DMA1_Channel3,USART3_RX_BUFF_SIZE);	//设置传输数据长度
        DMA_Cmd(DMA1_Channel3,ENABLE);				//打开DMA
			
        if(Usart3_Rx_Buf[0]!=0 && strlen((char *)Usart3_Rx_Buf) > 2)
        {
            Usart3_Rx_Buf[USART3_RX_BUFF_SIZE - 1]= 0;
            xQueueSendFromISR(xQueue_USART3_Cmd, (void *)Usart3_Rx_Buf,0);
            memset(Usart3_Rx_Buf, 0x00, USART3_RX_BUFF_SIZE);
        }
    }
}



/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

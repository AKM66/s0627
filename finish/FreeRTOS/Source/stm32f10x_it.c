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
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stdio.h"
#include "main.h"
#include "./usart/bsp_usart1.h"
#include "./usart/BSP_USART3.h" 	 
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
	  printf("---------HardFault_Handler-----------\r\n");
	  NVIC_SystemReset();
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
//void SVC_Handler(void)
//{
//}

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
//void PendSV_Handler(void)
//{
//}

//int i;
void UART4_IRQHandler(void)
{
//	uint16_t Res;
//	
//	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //????
//	{
//		Res = USART_ReceiveData(UART4);	//????????
////	data_handler(Res);
////		USART_SendData(USART1,Res);
////		xQueueSend(xQueue_Recv,Res,);
//	}
}
/**
  ******************************************************************************
  * @brief  串口中断函数
  * @retval None
  ******************************************************************************/
void USART1_IRQHandler(void)
{
//     uint8_t ch;
	
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
//        ch = USART_ReceiveData(USART1);
//		    USART1_SendChar(ch);
        
			  Usart1_Rx_Buf[RxCounter1++] = USART_ReceiveData(USART1);   //将读寄存器的数据缓存到接收缓冲区里
			  if(Usart1_Rx_Buf[RxCounter1-2]==0x0d&&Usart1_Rx_Buf[RxCounter1-1]==0x0a)//判断结束标志是否是0x0d 0x0a
				{
					Usart1_Rx_Buf[RxCounter1-2]='\0';
					Usart1_Rx_Buf[RxCounter1-1]='\0';
          xQueueSendFromISR(xQueue_USART1_Cmd, (void *)Usart1_Rx_Buf, 0);
					RxCounter1=0;
					memset(Usart1_Rx_Buf, 0, sizeof(Usart1_Rx_Buf));
				}
        /** Read one byte from the receive data register */
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
		
}
/*
****************************************************************************
*	函 数 名: TIM2_IRQHandler
*	功能说明: STM32 TIM2中断服务函数
*	形    参: 无
*	返 回 值: 无
****************************************************************************
*/
void TIM2_IRQHandler(void)
{
	static int cnt = 0;
	static float old_value = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查TIM2更新中断
	{

		if(cnt == 5)
		{
			if((result < 100) &&(result > 0))
			{
				Show_Temperature(result);
				old_value = result;
			}
			else{
				Show_Temperature(old_value);
				
			}
			cnt = 0;
		}
		cnt++;
//		cnt1++;
	}
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
}
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

/**
  ******************************************************************************
  * @brief  串口3中断函数
  * @retval None
  ******************************************************************************/
void USART3_IRQHandler(void)
{
    uint32_t len;

    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) //空闲总线中断
    {
        DMA_Cmd(DMA1_Channel3,DISABLE);
        USART3->SR;//先读SR
        USART3->DR; //读SR后读DR 清USART_IT_IDLE标志
			
        DMA_SetCurrDataCounter(DMA1_Channel3,USART3_RX_BUFF_SIZE);	//设置传输数据长度
        DMA_Cmd(DMA1_Channel3,ENABLE);								//打开DMA
        if(Usart3_Rx_Buf[0]!=0 && strlen((char *)Usart3_Rx_Buf) > 2)
        {
					//DMA_GetCurrDataCounter 获取传输过程中剩余的数据量
//            len = USART3_RX_BUFF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel3);      //计算实际接收数据长度
            Usart3_Rx_Buf[USART3_RX_BUFF_SIZE - 1]= 0;
            xQueueSendFromISR(xQueue_USART3_Cmd, (void *)Usart3_Rx_Buf,0);
            memset(Usart3_Rx_Buf, 0x00, USART3_RX_BUFF_SIZE);
        }

//        if(USART3_RX_BUFF_SIZE == len)
//        {
//            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//        }
//        else
//        {
//            DMA_SetCurrDataCounter(DMA1_Channel3,USART3_RX_BUFF_SIZE);	//设置传输数据长度
//            DMA_Cmd(DMA1_Channel3,ENABLE);								//打开DMA
//        }
    }
//    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
//    {
//        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//        USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);

//        DMA_SetCurrDataCounter(DMA1_Channel3,USART3_RX_BUFF_SIZE);	//设置传输数据长度
//        DMA_Cmd(DMA1_Channel3,ENABLE);								//打开DMA
//    }
}


/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

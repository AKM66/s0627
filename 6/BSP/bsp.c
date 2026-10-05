/**
  ******************************************************************************
  * @file    common.c
  * @author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   This file contains all common init.
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
#include "bsp.h"
#include "lcd.h"
#include "lcd_init.h"
#include "pic.h"
#include "pic4.h"
#include "BSP_Delay.h"
#include "LED.H"
#include "stdbool.h"
#include "OLED.h"
#include "BSP_DS18B20.h"
#include "BSP_DigitalTube.h"
#include "BSP_Buzzer.h"
#include "BSP_UART.h" 	 
#include "BSP_Timer.h"


void ReadChipID(char* cpId, int len)
{
    uint32_t ChipId[3] = {0};

    ChipId[0] = *(uint32_t *)(0x1ffff7e8);
    ChipId[1] = *(uint32_t *)(0x1ffff7ec);
    ChipId[2] = *(uint32_t *)(0x1ffff7f0);

    snprintf(cpId, len, "%08x%08x%08x", ChipId[0], ChipId[1], ChipId[2]);
}


void COMMON_Show_Info(void)
{
    char InfoBuffer[256];
    vTaskList(InfoBuffer);				
    PRINTF_LOG("\r\n-------------------------------------------\r\n");
    PRINTF_LOG("Name\t\tState\tPrior\tStack\tNum\r\n");
    PRINTF_LOG("-------------------------------------------\r\n");
    PRINTF_LOG("%s",InfoBuffer);
    PRINTF_LOG("-------------------------------------------\r\n");
    PRINTF_LOG("   ** Usable RAM size is %d Bytes **\r\n", xPortGetFreeHeapSize());
    PRINTF_LOG("-------------------------------------------\r\n");
}


void BSP_Init(void)
{
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	BSP_Delay_Init();

	OLED_Init();			//��ʼ��OLED
	OLED_Clear();
	BSP_DS18B20_Init();
	BSP_DigitalTube_Init();
	BSP_LEDBuzzer_Init();

	BSP_Timer_Init(1*1000);	//��ʱ 1*1000us = 1ms
	KEY_Init(COMMON_Show_Info,ReconfigWIFI);
	
	USART1_Init(BSP_USART_BAUDRATE_115200);
	USART3_Init(BSP_USART_BAUDRATE_115200);

	PRINTF_LOG("\r\n===========================================\r\n");
	PRINTF_LOG("FreeRTOS Version : %s\r\n", tskKERNEL_VERSION_NUMBER);
	PRINTF_LOG("Compile Time : %s  %s\r\n", __DATE__, __TIME__);
	PRINTF_LOG("===========================================\r\n");

   
}

/*********************************************END OF FILE**********************/


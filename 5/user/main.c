/****************************************************************
				   Source file of main.c
****************************************************************/
#include "public.h"
#include "stm32f10x.h"
#include "BSP_Delay.h"
#include "BSP_DS18B20.h"
#include "BSP_DebugUART.h"
#include "BSP_Buzzer.h"
#include "BSP_DigitalTube.h"
#include "BSP_Timer.h"
#include "stm32f10x_it.h"
#include "OLED.H"
#include "BSP_UART.h"
#include "Queue.h"
#include "FreeRTOS.h"
#include "UserTask.h"

#include "LED.H"

// define three extern variable
u8 hour, minute, second;

/****************************************************************
 * Function Name  : main
 * Description    : Main program.
 * Input          : None
 * Output         : None
 * Return         : None
 ****************************************************************/
int main()
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	BSP_Delay_Init();
	BSP_DebugUART_Init(115200);
	BSP_DS18B20_Init();
	DsgShowInit();
	// DsgShowTime();
	BSP_Buzzer_Init();
	BSP_DigitalTube_Init();
	BSP_Timer_Init(1000);
	BeepInit();

	LEDInit();

	OLED_Init();
	OLED_Clear();

	OLED_ShowCHinese(1, 0, 0);		  // s
	OLED_ShowCHinese(18, 0, 1);		  // x
	OLED_ShowCHinese(35, 0, 2);		  // n
	OLED_ShowCHinese(52, 0, 3);		  // y
	OLED_ShowCHinese(69, 0, 4);		  // 大
	OLED_ShowCHinese(86, 0, 5);		  // 学
	OLED_Show_CHinese32X32(8, 2, 0);  //
	OLED_Show_CHinese32X32(48, 2, 1); //

	BSP_USART_Init(115200);

	UserTaskCreate();

	
	GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

	while (1)
	{
		printf("%f", result); //"TXD-PA9 RXD-PA10 GND"
		SenSorData_Output();
		BSP_Delay_ms(2000);
		// Show_Temperature(result);
	}
}

//

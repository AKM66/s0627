/****************************************************************
                   Source file of main.c
****************************************************************/
#include "public.h"
#include "BSP_Delay.h"
#include "BSP_DS18B20.h"
#include "BSP_DebugUART.h"
#include "BSP_Buzzer.h"
#include "BSP_Timer.h"
#include "BSP_UART.h"
#include "OLED.H"
#include "UserTask.h"


//define three extern variable
u8 hour,minute,second;


/****************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************/
int main()
{
	u8 Email[]="1414782@qq.com";
	//hour=9;
	//minute=30;
	//second=15;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	BSP_Delay_Init();
	BSP_DebugUART_Init(115200);
	BSP_USART3_Init(115200);
	
	BSP_Buzzer_Init();
	DsgShowInit();
	LEDInit();
	
		OLED_Init();			//初始化OLED  
		OLED_Clear()  	;

		OLED_ShowCHinese(0,0,4);//山
		OLED_ShowCHinese(20,0,5);//西
		OLED_ShowCHinese(40,0,6);//农
		OLED_ShowCHinese(60,0,7);//业
		OLED_ShowCHinese(80,0,8);//大
		OLED_ShowCHinese(100,0,9);//学
	
		OLED_Show_CHinese32X32(8,2,3);//张
		OLED_Show_CHinese32X32(48,2,4);	//紫
		OLED_Show_CHinese32X32(88,2,5);	//豪
		OLED_ShowString(4,6,Email);
	//DsgShowTime();

   UserTaskCreate ();
	

//	while(1)
//	{
//		SenSorData_Output();
////		if(result > 100.0f)
////		{
////		 sound1();
////		}
//		//Show_Temperature(result);
//		BSP_Delay_ms(2000);
//	}
}

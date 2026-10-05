#include "BSP_Buzzer.h"

void BSP_Buzzer_Init(void)
{
		
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); 
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	BSP_Buzzer_Off();
}	

void BSP_Buzzer_On(void) 
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);
}

void BSP_Buzzer_Off(void) 
{
	GPIO_SetBits(GPIOC,GPIO_Pin_8);
}

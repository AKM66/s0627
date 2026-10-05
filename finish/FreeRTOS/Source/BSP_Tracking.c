#include "BSP_Tracking.h"
//#include "BSP_DebugUART.h"
#include "./usart/bsp_usart1.h"

volatile char trackbuf[20] = {0};
void BSP_TrackingModule_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|\
	RCC_APB2Periph_GPIOD, ENABLE); 
	//OUT4 PB7	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	//OUT5 PB8	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 	
	
	//OUT1 PC11	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	

	//OUT2 PC12	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	
	
	//OUT2 PD2	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
}

void Print_Tracking_Status(void)
{
	printf("%ld  %ld  %ld  %ld  %ld \r\n",OUT1,OUT2,OUT3,OUT4,OUT5);

	if(((OUT2) && (OUT3) && (OUT4)) || ((!OUT2) && (!OUT3) && (!OUT4)))
	{
		printf("stop\r\n");
		memset((char *)trackbuf,0,20);	
		strcpy((char *)trackbuf,"stop");
	}
	if(((OUT2) && (!OUT3) && (OUT4)))
	{
		printf("ahead\r\n");
		memset((char *)trackbuf,0,20);
		strcpy((char *)trackbuf,"ahead");
	}
	if(((!OUT2) && (OUT3) && (OUT4) ))
	{
		printf("left\r\n");
		memset((char *)trackbuf,0,20);
		strcpy((char *)trackbuf,"left");
	}
	if(((OUT2) && (OUT3) && (!OUT4) ))
	{
		printf("right\r\n");
		memset((char *)trackbuf,0,20);
		strcpy((char *)trackbuf,"right");
	}

}

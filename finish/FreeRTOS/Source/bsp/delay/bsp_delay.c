#include "stm32f10x.h"
#include "./delay/bsp_delay.h"


//ÑÓÊ±ÏµÊý
unsigned char UsCount = 0;
unsigned short MsCount = 0;



void Delay_Init(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	TIM6->CR1 |= 1UL << 3;									//µ¥Âö³åÄ£Ê½

	TIM6->EGR |= 1;											//¸üÐÂÉú³É

	TIM6->DIER = 0;											//½ûÖ¹ËùÓÐÖÐ¶Ï

	TIM6->CR1 &= (unsigned short)~TIM_CR1_CEN;				//Í£Ö¹¼ÆÊ±

}


void DelayUs(unsigned short us)
{

	TIM6->ARR = us;

	TIM6->PSC = 71;									//timer6Îª72MHz£¬ÉèÖÃÎª71+1·ÖÆµ£¬1MHz£¬1us¼ÆÊýÒ»´Î

	TIM6->CR1 |= (unsigned short)TIM_CR1_CEN;		//¿ªÊ¼¼ÆÊ±

	while (!(TIM6->SR & 1));							//µÈ´ý²úÉúÖÐ¶ÏÊÂ¼þ

	TIM6->SR &= ~(1UL << 0);						//Çå³ý±êÖ¾

}


void DelayXms(unsigned short ms)
{

	if (ms < 32768)
	{
		TIM6->ARR = (ms << 1);						//Ë«±¶¼ÆÊýÖµ

		TIM6->PSC = 35999;							//timer6Îª72MHz£¬ÉèÖÃÎª35999+1·ÖÆµ£¬2KHz£¬500us¼ÆÊýÒ»´Î

		TIM6->CR1 |= (unsigned short)TIM_CR1_CEN;	//¿ªÊ¼¼ÆÊ±

		while (!(TIM6->SR & 1));						//µÈ´ý²úÉúÖÐ¶ÏÊÂ¼þ

		TIM6->SR &= ~(1UL << 0);					//Çå³ý±êÖ¾
	}

}


void DelayMs(unsigned short ms)
{

	unsigned char repeat = 0;
	unsigned short remain = 0;

	repeat = ms / 500;
	remain = ms % 500;

	while (repeat)
	{
		DelayXms(500);
		repeat--;
	}

	if (remain)
		DelayXms(remain);

}


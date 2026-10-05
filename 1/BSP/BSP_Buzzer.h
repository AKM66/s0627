#ifndef __BSP_BUZZER_H
#define __BSP_BUZZER_H

#include "stm32f10x.h"
#include "systick.h"

#define RCC_Buzzer			(RCC_APB2Periph_GPIOC)
#define GPIO_PORT_Buzzer	GPIOC
#define GPIO_PIN_Buzzer		(GPIO_Pin_8)

void BSP_Buzzer_Init(void);

void BSP_Buzzer_On(void);
void BSP_Buzzer_Off(void);
void BSP_Buzzer_Toggle(void);

#endif

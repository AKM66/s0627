#ifndef __BSP_SMOKE_H
#define __BSP_SMOKE_H

#include "stm32f10x.h"
#include "stdbool.h"

#define RCC_SMOKE				(RCC_APB2Periph_GPIOA)
#define GPIO_PORT_SMOKE		GPIOA
#define GPIO_PIN_SMOKE		(GPIO_Pin_7)

void BSP_SMOKE_Init(void);
bool BSP_SMOKE_Scan(void);

#endif

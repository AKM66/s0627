#ifndef __BSP_BUZZER_H
#define __BSP_BUZZER_H

#include "stm32f10x.h"
#include "systick.h"

#define RCC_Buzzer			(RCC_APB2Periph_GPIOC)
#define GPIO_PORT_Buzzer	GPIOC
#define GPIO_PIN_Buzzer		(GPIO_Pin_8)

#define LED					((uint8_t)0x01)
#define Buzzer				((uint8_t)0x02)


#define RCC_LED				(RCC_APB2Periph_GPIOC)
#define GPIO_PORT_LED		GPIOC
#define GPIO_PIN_LED0		(GPIO_Pin_0)
#define GPIO_PIN_LED1		(GPIO_Pin_1)
#define GPIO_PIN_LED2		(GPIO_Pin_2)
#define GPIO_PIN_LED3		(GPIO_Pin_3)
#define GPIO_PIN_LED4		(GPIO_Pin_4)
#define GPIO_PIN_LED5		(GPIO_Pin_5)
#define GPIO_PIN_LED6		(GPIO_Pin_6)
#define GPIO_PIN_LED7		(GPIO_Pin_7)
#define RCC_Buzzer			(RCC_APB2Periph_GPIOF)
#define GPIO_PORT_Buzzer	GPIOF
#define GPIO_PIN_Buzzer		(GPIO_Pin_11)

void BSP_Buzzer_Init(void);

void BSP_LED_On(uint8_t number);
void BSP_LED_Off(uint8_t number);

void BSP_Buzzer_On(void);
void BSP_Buzzer_Off(void);
void BSP_Buzzer_Toggle(void);

#endif

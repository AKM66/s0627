#ifndef _LED_H
#define _LED_H
#include "stm32f10x.h"

#include "systick.h"

extern uint16_t LED_Pin[8];
void LEDInit(void);
void LEDdisplay(void);
void BSP_LED_Control(uint8_t LEDNumber);

#endif

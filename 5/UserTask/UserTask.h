#ifndef _USERTASK_H_
#define _USERTASK_H_

#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "e:\progect_private\arm32\shixun\s0627\4\FreeRTOS\include\queue.h"

extern xQueueHandle xQueue_USART3_Cmd;
extern char gUsartReciveLineBuf[200];

void UserTaskCreate(void);
static void vTaskLED(void *pvParameters);

#endif

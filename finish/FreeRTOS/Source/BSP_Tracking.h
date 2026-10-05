#ifndef __BSP_TRACKING_H__
#define __BSP_TRACKING_H__

#include "stm32f10x.h"

#define OUT1 PCin(11)

#define OUT2 PCin(12)
#define OUT3 PDin(2)
#define OUT4 PBin(7)

#define OUT5 PBin(8)
extern volatile char trackbuf[20];
void BSP_TrackingModule_Init(void);

void Print_Tracking_Status(void);

#endif

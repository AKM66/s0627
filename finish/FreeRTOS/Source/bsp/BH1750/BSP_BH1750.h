#ifndef __BSP_BH1750_H
#define __BSP_BH1750_H

#include "stm32f10x.h"
#include "BSP_MyIIC.h"

#define BH1750_SUCCESS                           ((uint8_t)1)
#define BH1750_FAILURE                           ((uint8_t)0)

#define BH1750_Addr                              ((uint8_t)0x46)

#define BH1750_Power_Down                        ((uint8_t)0x00)
#define BH1750_Power_On                          ((uint8_t)0x01)
#define BH1750_Reset                             ((uint8_t)0x07)
#define BH1750_Continuously_HResolution_Mode     ((uint8_t)0x10)
#define BH1750_Continuously_HResolution_Mode2    ((uint8_t)0x11)
#define BH1750_Continuously_LResolution_Mode     ((uint8_t)0x13)
#define BH1750_OneTime_HResolution_Mode          ((uint8_t)0x20)
#define BH1750_OneTime_HResolution_Mode2         ((uint8_t)0x21)
#define BH1750_OneTime_LResolution_Mode          ((uint8_t)0x23)

//MT Register 请按 datasheet 要求赋值！
//#define BH1750_MeasurementTime_Reg_H
//#define BH1750_MeasurementTime_Reg_L

#define BH1750_DVI			PCout(1)

void BSP_BH1750_Init(void);

uint8_t BSP_BH1750_SetMode(uint8_t BH1750_Mode);
uint16_t BSP_BH1750_ReadResult(void);

#endif

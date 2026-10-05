#ifndef _BEEPKEY_H
#define _BEEPKEY_H
#include "stm32f10x.h"
#include "systick.h"
void delay(u32 i);
void BeepInit(void);	 
void KeyInit(void);
void sound1(void) ;
void sound2(void);
void BeepKey(void);

#endif



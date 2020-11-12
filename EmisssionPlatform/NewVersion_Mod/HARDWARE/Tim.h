#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f10x.h"

void TIM2_Config(void);
void TIM5_Cap_Init(void);
void Get_Encode_Speed(void);
u32 Get_us10(void);
void us10_reset(void);

#endif

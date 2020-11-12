#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f10x.h"

void GPIO_Photocell_1(void);
void GPIO_Photocell_2(void);
u8 Get_Photocell_1(void);
u8 Get_Photocell_2(void);
u32 Photocell_Detect(u8 Num);

#endif

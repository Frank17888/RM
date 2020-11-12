#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"

void LED_Config(void);
void LedStatus0(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus1(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus2(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus3(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus4(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus5(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus6(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus7(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus8(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus9(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus10(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus11(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void LedStatus12(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
void Led_Twinkle1(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t Period,uint16_t On);
void Led_Twinkle2(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t Period,uint16_t On);
void Disconnect_Report(u32 Disconnect_Flag);

#endif

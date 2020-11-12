#include "main.h"
/** @brief 释放舵机配置
  *
	*
	**/
void MG995_TIM5_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIMOC;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_0;    //channel2 和 channel1 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_DeInit(TIM5);
	 
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 200-1; //即ARR，计数两百次即为20ms 
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;//72000000/(7200)=10KHz,0.1ms计数一次
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	//输出比较功能
	TIMOC.TIM_OCMode=TIM_OCMode_PWM1;
	TIMOC.TIM_OutputState=TIM_OutputState_Enable;
	TIMOC.TIM_OutputNState=TIM_OutputNState_Disable;
	TIMOC.TIM_Pulse=15;//1.5ms脉宽为角度为0位置，也是计数值
	TIMOC.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM5, &TIMOC); 
	TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM5,ENABLE);
	TIM_Cmd(TIM5,ENABLE);
}


/**
  * @brief  pitch推杆电机配置
  * @param  None
  * @retval None 	
  */ 
void Pitch_TIM3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 
  TIM_DeInit(TIM3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3 , ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 14400-1;//周期0.002s
	TIM_TimeBaseStructure.TIM_Prescaler =10-1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 
	
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); 
	TIM_Cmd(TIM3, ENABLE); 
}
/**
  * @brief  推杆电机配置
  * @param  None
  * @retval None 	
  */ 
void Motor_Putter_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  TIM_DeInit(TIM4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	TIM_TimeBaseStructure.TIM_Period = 14400-1;//pwm周期0.002s
	TIM_TimeBaseStructure.TIM_Prescaler =10-1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); 
	TIM_Cmd(TIM4, ENABLE); 
}
/**
  * @brief  履带电机控制
	* @param  speed:0~7400
	*					polar:0/1
  * @retval None 	
  */ 
/**
  * @brief  换弹支架电机控制/推杆电机
	* @param  updown:1 上 0 下//原来1上2下
  * @retval None 	
  */ 
void Putter_Move_Updown(int updown)
{
	if(updown)
	{
		TIM_SetCompare2(TIM4,14000);//占空比14000/14400=35/36
		TIM_SetCompare1(TIM4,0);
	}
	else
	{
		TIM_SetCompare2(TIM4,0);
		TIM_SetCompare1(TIM4,14000);
	}
}


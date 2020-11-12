#include "main.h"

u32 Out_Or_Back;
u32 Triggercnt;//奇数为上升沿，偶数为下降沿
u32 usget;
u8 MissileUp_Flag=0;
float us;
float Lspeed;


/**
  * @brief  光电管1配置,包含外部中断测速
  * @param  None
  * @retval None 	
  */ 
void GPIO_Photocell(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
	//配置外部中断
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);
		
		EXTI_InitTypeDef EXTI_InitStructure;//定义初始化结构体
		
		EXTI_InitStructure.EXTI_Line=EXTI_Line3; 
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;

		EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
	}
	//中断优先级
	{
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
		
		NVIC_InitTypeDef NVIC_InitStructure;//定义结构体
		NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; //使能外部中断所在的通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级 0， 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //子优先级 0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道 

		NVIC_Init(&NVIC_InitStructure); //根据结构体信息进行优先级初始化
	}
}
/**
  * @brief  光电管2配置
  * @param  None
  * @retval None 	
  */ 

/**
  * @brief  获取光电管1信号
  * @param  None
  * @retval 1为遮挡，0为无遮挡
  */ 
u8 Get_Photocell_1(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3);
}
/**
  * @brief  获取光电管2信号
  * @param  None
  * @retval 1为遮挡，0为无遮挡
  */ 
u8 Get_Photocell_2(void)
{
	return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11);
}
/**
  * @brief  光电管1外部中断测速
  * @param  None
  * @retval None
  */ 
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)//判断某个线上的中断是否发生 
	{
		EXTI_ClearITPendingBit(EXTI_Line3);   //清除 LINE 上的中断标志位
		Triggercnt ++;
		if(Triggercnt % 2 == 0)//2的整数倍
		Out_Or_Back ++;  
		if(((Out_Or_Back%2)) && !(Triggercnt%2)) //发射架发出单程，无返回
		{
			usget = Get_us10();
			us = (float)usget;
			Lspeed = 1000 * 65.6f / us;
			us10_reset();
			MissileUp_Flag=1;
		}
	}     
}
/**
  * @brief  光电管2外部中断
  * @param  None
  * @retval None
  */ 

/**
  * @brief  光电管检测滑块发出或倒回
		Out_Or_Back就是发出回来信号，发出/回来一个过程遮挡两次，Out_Or_Back+1
	* @param  Num:滑块编号1/2
	* @retval 1:发出，0：倒回,8:错误	2020.11.5
   */ 

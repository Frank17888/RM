#include "main.h"

u32 Out_Or_Back1,Out_Or_Back2;
u32 Triggercnt1,Triggercnt2;//奇数为上升沿，偶数为下降沿
u32 usget;
float us;
float Lspeed;


/**
  * @brief  光电管1配置,包含外部中断测速
  * @param  None
  * @retval None 	
  */ 
void GPIO_Photocell_1(void)
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
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
		
		NVIC_InitTypeDef NVIC_InitStructure;//定义结构体
		NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; //使能外部中断所在的通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级 0， 
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
void GPIO_Photocell_2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_SetBits(GPIOC,GPIO_Pin_10);
	//配置外部中断
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
		
		EXTI_InitTypeDef EXTI_InitStructure;//定义初始化结构体
		
		EXTI_InitStructure.EXTI_Line=EXTI_Line11; 
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;

		EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
	}
	//中断优先级
	{
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
		
		NVIC_InitTypeDef NVIC_InitStructure;//定义结构体
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //使能外部中断所在的通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级 0， 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //子优先级 0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道 

		NVIC_Init(&NVIC_InitStructure); //根据结构体信息进行优先级初始化
	}
}
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
		Triggercnt1 ++;
		if(Triggercnt1 % 2 == 0)//2的整数倍
		Out_Or_Back1 ++;  
		us10_on_off = (u8)(Out_Or_Back1 % 2);//奇数时计数，偶数时结束
		if((!us10_on_off) && Triggercnt1 == 4) 
		{
			usget = Get_us10();
			us = (float)usget;
			Lspeed = 1000 * 65.6f / us;
			us10_reset();
		}
	}     
}
/**
  * @brief  光电管2外部中断
  * @param  None
  * @retval None
  */ 
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)//判断某个线上的中断是否发生 
	{
		EXTI_ClearITPendingBit(EXTI_Line11);   //清除 LINE 上的中断标志位
		Triggercnt2 ++;
		if(Triggercnt2 % 2 == 0)//2的整数倍
		Out_Or_Back2 ++;  //偶数的时候说明是倒回
	}     
}
/**
  * @brief  光电管检测滑块发出或倒回
	* @param  Num:滑块编号1/2
	* @retval 1:倒回，0：发出,8:错误
  */ 
u32 Photocell_Detect(u8 Num)
{
	switch(Num)
	{
		case 1:
			if(Out_Or_Back1 % 2 == 0)
				return 1;
			else
				return 0;
		case 2:
			if(Out_Or_Back2 % 2 == 0)
				return 1;
			else 
				return 0;
		default:
			return 8;
	}
}

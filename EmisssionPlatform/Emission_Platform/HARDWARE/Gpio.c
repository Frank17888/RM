#include "main.h"

u32 Out_Or_Back1,Out_Or_Back2;
u32 Triggercnt1,Triggercnt2;//����Ϊ�����أ�ż��Ϊ�½���
u32 usget;
float us;
float Lspeed;


/**
  * @brief  ����1����,�����ⲿ�жϲ���
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
	//�����ⲿ�ж�
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);
		
		EXTI_InitTypeDef EXTI_InitStructure;//�����ʼ���ṹ��
		
		EXTI_InitStructure.EXTI_Line=EXTI_Line3; 
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;

		EXTI_Init(&EXTI_InitStructure);//���ݽṹ����Ϣ���г�ʼ��
	}
	//�ж����ȼ�
	{
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
		
		NVIC_InitTypeDef NVIC_InitStructure;//����ṹ��
		NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; //ʹ���ⲿ�ж����ڵ�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //��ռ���ȼ� 0�� 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //�����ȼ� 0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ�� 

		NVIC_Init(&NVIC_InitStructure); //���ݽṹ����Ϣ�������ȼ���ʼ��
	}
}
/**
  * @brief  ����2����
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
	//�����ⲿ�ж�
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
		
		EXTI_InitTypeDef EXTI_InitStructure;//�����ʼ���ṹ��
		
		EXTI_InitStructure.EXTI_Line=EXTI_Line11; 
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;

		EXTI_Init(&EXTI_InitStructure);//���ݽṹ����Ϣ���г�ʼ��
	}
	//�ж����ȼ�
	{
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
		
		NVIC_InitTypeDef NVIC_InitStructure;//����ṹ��
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //ʹ���ⲿ�ж����ڵ�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ� 0�� 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //�����ȼ� 0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ�� 

		NVIC_Init(&NVIC_InitStructure); //���ݽṹ����Ϣ�������ȼ���ʼ��
	}
}
/**
  * @brief  ��ȡ����1�ź�
  * @param  None
  * @retval 1Ϊ�ڵ���0Ϊ���ڵ�
  */ 
u8 Get_Photocell_1(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3);
}
/**
  * @brief  ��ȡ����2�ź�
  * @param  None
  * @retval 1Ϊ�ڵ���0Ϊ���ڵ�
  */ 
u8 Get_Photocell_2(void)
{
	return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11);
}
/**
  * @brief  ����1�ⲿ�жϲ���
  * @param  None
  * @retval None
  */ 
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)//�ж�ĳ�����ϵ��ж��Ƿ��� 
	{
		EXTI_ClearITPendingBit(EXTI_Line3);   //��� LINE �ϵ��жϱ�־λ
		Triggercnt1 ++;
		if(Triggercnt1 % 2 == 0)//2��������
		Out_Or_Back1 ++;  
		us10_on_off = (u8)(Out_Or_Back1 % 2);//����ʱ������ż��ʱ����
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
  * @brief  ����2�ⲿ�ж�
  * @param  None
  * @retval None
  */ 
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)//�ж�ĳ�����ϵ��ж��Ƿ��� 
	{
		EXTI_ClearITPendingBit(EXTI_Line11);   //��� LINE �ϵ��жϱ�־λ
		Triggercnt2 ++;
		if(Triggercnt2 % 2 == 0)//2��������
		Out_Or_Back2 ++;  //ż����ʱ��˵���ǵ���
	}     
}
/**
  * @brief  ���ܼ�⻬�鷢���򵹻�
	* @param  Num:������1/2
	* @retval 1:���أ�0������,8:����
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

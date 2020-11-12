#include "main.h"

u32 Out_Or_Back;
u32 Triggercnt;//����Ϊ�����أ�ż��Ϊ�½���
u32 usget;
u8 MissileUp_Flag=0;
float us;
float Lspeed;


/**
  * @brief  ����1����,�����ⲿ�жϲ���
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
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
		
		NVIC_InitTypeDef NVIC_InitStructure;//����ṹ��
		NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; //ʹ���ⲿ�ж����ڵ�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ� 0�� 
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
		Triggercnt ++;
		if(Triggercnt % 2 == 0)//2��������
		Out_Or_Back ++;  
		if(((Out_Or_Back%2)) && !(Triggercnt%2)) //����ܷ������̣��޷���
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
  * @brief  ����2�ⲿ�ж�
  * @param  None
  * @retval None
  */ 

/**
  * @brief  ���ܼ�⻬�鷢���򵹻�
		Out_Or_Back���Ƿ��������źţ�����/����һ�������ڵ����Σ�Out_Or_Back+1
	* @param  Num:������1/2
	* @retval 1:������0������,8:����	2020.11.5
   */ 

#include "main.h"
/*
���ţ�
PC6,7:�Ĵ����
PA0,1:������
PA2������1���չ�3.3V����
PA3������1���չ��ź�ͨ��
PC10������2���չ�3.3V����
PC11������2���չ��ź�ͨ��
��������
*/
u32 syscnt;
u32 Disconnect_Flag_All;//���߹۲��־λ
Disconnect_Flag Discnect_Flag;//���߽ṹ��
Status_ Status;
Lauching_States LauchState;//�Ĵ�����
RCC_ClocksTypeDef rcc;

extern u8 us10_on_off;
extern u32 Triggercnt1,us10cnt,Triggercnt2;
extern u8 RechargeStartOver;
int testspeed,testspeed2=0;
int test = 1;
int testflag=0;

int main()
{
	RCC_GetClocksFreq(&rcc);
/***********��������***********/
	LED_Config();
	Motor_Config();
	Motor_Putter_Config();
	Solenoid_Config();
	TIM2_Config();
	TIM5_Cap_Init();
	TIM7_Configration();
	delay_ms(1000);
	GPIO_Photocell_1();
	GPIO_Photocell_2();
/************ͨ��***************/	
	USART_Config();
	CAN1_Config();//��̨3508
	delay_ms(1000);
///************������ʼ��***************/
	ZeroCheck_Init();
	Pid_Speed_Init();
	Pid_Gimbal_Pos_Init();
	Pid_Gimbal_Speed_Init();
	rc_reset();
	//???????�ڹ��ܳ�ʼ��֮���ֱ�ӽ���ֻ��һ���жϣ�����������24v��Ҳ�ᵼ�½�һ���ⲿ�ж�
	{
		Triggercnt1 = 0;
		us10_on_off = 0;
		us10cnt = 0;
		Triggercnt2 = 0;
	}
/***************Systick********************/
	SysTick_Configuration();
	while(1)
	{
//		Motor_Speed_Set(0,1);
//		if(testflag == 1)
//		{
//			Test_Task();
//			testflag = 0;
//		}
	}
}

void SysTick_Handler(void)
{
///**********************������***************************/
	syscnt ++;
///********************���и���***************************/
////	Disconnect_Flag_All = Offline_Detect(&Discnect_Flag);//��ȡ������Ϣ
	Get_Encode_Speed();
//	ZeroCheck_cal();
//	ZeroCheck_Excution();
///**************����״̬����*****************************/
////	Disconnect_Report(Disconnect_Flag_All);//led��ӳ״̬
//	Remote_Convert(rc_ctrl);
//	Status_Act(Status);
///*********************�������**************************/
//	Motor_Cal();
//	Pid_Speed_Cal(); 
//	Pid_Gimbal_Cal_Rc();
//	Putter_Move_Updown(test);
//	if(RechargeStartOver)
//	Recharge_Task();
//	if(RechargeStartOver )
//	{
//		Recharge_Task();
//	}
//	Motor_Speed_Set(testspeed,1);//������������ⳳ
testspeed = (rc_ctrl.rc.ch3 - 1024) * 11.0f;
Motor_Speed_Set(testspeed,1);
if(rc_ctrl.rc.s2 == 1)
{
	PushToTrack = 1;
}
else
{
	PushToTrack = 0;
}
}
/**
  * @brief  Systick��ʼ��
  * @param  None
  * @retval None
  */
void SysTick_Configuration(void)
{
  /* Setup SysTick Timer for 10 msec interrupts  */
  if (SysTick_Config(72000000/ 1000))                //SysTick���ú��� T = 1 / 1000 = 1ms
  { 
    /* Capture error */ 
    while (1);
  }  
 /* Configure the SysTick handler priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  NVIC_SetPriority(SysTick_IRQn, 0x0);                       //SysTick�ж����ȼ�����
}
/**
  * @brief  ��ȡu32�ĵ�nλ(���߱�־��)
  * @param  num32��u32����
	*					n:��nλ
  * @retval u32�ĵ�nλ
  */
u32 Get_u32_nth(u32 num32,u8 n)
{
	if(n < 32)
	{
		return ((num32 >> n) & 0x0001);
	}
	else 
	{
		return 0;
	}
}
/**
  * @brief  ��ȡ����ʱ��
  * @param  None
  * @retval ϵͳ����ʱ��ms
  */
u32 GetSyscnt(void)
{
	return syscnt;
}

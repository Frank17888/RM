#include "main.h"
/*
���ţ�
PC6,7:�Ĵ����																					ң����ģ�ͣ� s2														s1
PA0,1:�ͷŶ��																														ch3								ch1
PA2������1���չ�3.3V����																										ch2								ch0
PA3������1���չ��ź�ͨ��																									��ң������s2��s1λ���෴
//PC10������2���չ�3.3V����																			s2���ϵ��£�����ҡ�ˡ�����
//PC11������2���չ��ź�ͨ��																			s1���ϵ��£�����̨������̨����	
��������																												ch3���Ĵ�ת�١�ch1����
																															����������Զ�������׼���ڶ��η���
																																
���ȼ����ã�Tim6��7��us��ms��ʱ0 0 ��0 1
						���ܲ�1 0
						TIM2  ������   1 1 
						can���ա����䣺0 0��1��0		
�������0,1,2 �ֱ��Ӧ PushPull��Recharge��yaw
*/			
/*
10.17�� debugʱ����ֱ��watch CurrentSend[0]�۲컻������ĵ�������0x201
*/
u32 syscnt;
u32 Disconnect_Flag_All;//���߹۲��־λ
Disconnect_Flag Discnect_Flag;//���߽ṹ��
Status_ Status;
Lauching_States LauchState;//�Ĵ�����
RCC_ClocksTypeDef rcc;
u8 AutoCtrl_Flag;
extern int CurrentSend[MotorNum];
extern u32 Triggercnt,us10cnt;//����Ϊ�����أ�ż��Ϊ�½���
extern u8 RechargeWork_Flag;

int main()
{
  RCC_GetClocksFreq(&rcc);
/*--------------------------��������-----------------------------*/
	LED_Config();
	MG995_TIM5_Config();//���ض����ʼ��
	Pitch_TIM3_Config();//pitch�Ƹ˵����ʼ��
	Motor_Putter_Config();//�Ƹ˳�ʼ��
	TIM2_Config();//��Ϲ���10us��ʱ

	TIM7_Config();//���뼶��ʱ����
	delay_ms(1000);
	GPIO_Photocell();//��������

/*------------------------------ͨ��------------------------------*/	
	USART_Config();//���ڳ�ʼ��
	CAN1_Config();//��̨3508��6020
	delay_ms(1000);
/*---------------------------������ʼ��---------------------------*/
	ZeroCheck_Init();
	Pid_Gimbal_Pos_Init();//���λ�û���ʼ��
	Pid_Gimbal_Speed_Init();//����ٶȻ���ʼ��
	rc_reset();
	Flag_Initialize();
	SysTick_Configuration();
//	//???????�ڹ��ܳ�ʼ��֮���ֱ�ӽ���ֻ��һ���жϣ�����������24v��Ҳ�ᵼ�½�һ���ⲿ�ж�
	{
		Triggercnt = 0;
		us10_on_off = 0;
		us10cnt = 0;		
	}

	while(1)
	{
		
	}	
}
		


void SysTick_Handler(void)
{
/*----------------------------------������----------------------------------*/
	syscnt ++;
/*-----------------------�鿴�Ƿ���뵹�ػ�������-----------------------------------*/
	CheckAutoCtrl();
	PullWork_Task();
	Recharge_Task();
/*-----------------------------ϵͳ�������-----------------------------------*/
	Disconnect_Flag_All = Offline_Detect(&Discnect_Flag);//��ȡ������Ϣ,����ͨ���۲���������жϣ�
	Disconnect_Report(Disconnect_Flag_All);//led��ӳ����״̬
	ZeroCheck_cal();//�����λ�û������⣬���� Motor_Angle[i]
	ZeroCheck_Excution();//��������������λ�ã����Ϊ RealPos[i]
	Remote_Convert(rc_ctrl);//ͨ��ң�ؿ��Ƽ���/ң��/����״̬�л�����
	Status_Act(Status);//״̬�������ã���������̨���������ʵ�ʿ���(����ң�ء�����)
/*----------------------------������������----------------------------------*/	
	Pid_Gimbal_Cal_Rc();//��̨λ�û����ٶȻ������������pitch��yaw��pid����
/*-------------------------������ѹ����������3508��6020------------------------*/
	Gimbal_Current_Voltage_Send(CurrentSend[0],CurrentSend[1],0/*CurrentSend[3]_pitch*/,0,CurrentSend[2],0,0,0);
//yaw����Ϊ6020����Ҫ��������
}
/**
  * @brief  Systick��ʼ��
  * @param  None
  * @retval None
  */
void SysTick_Configuration(void)
{
  /* Setup SysTick Timer for 5 msec interrupts  */
  if (SysTick_Config(72000000/ 2000))                //SysTick���ú��� T = 1 / 2000 = 0.5ms
  { 
    /* Capture error */ 
    while (1);
  }  
 /* Configure the SysTick handler priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  NVIC_SetPriority(SysTick_IRQn, 0x00);                       //SysTick�ж����ȼ�����
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

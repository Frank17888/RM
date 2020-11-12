#include "main.h"
/*
���ţ�
PC6,7:�Ĵ����																					ң����ģ�ͣ� s2														s1
PA0,1:������																															ch3								ch1
PA2������1���չ�3.3V����																										ch2								ch0
PA3������1���չ��ź�ͨ��																									��ң������s2��s1λ���෴
PC10������2���չ�3.3V����																			s2���ϵ��£�����ҡ�ˡ�����
PC11������2���չ��ź�ͨ��																			s1���ϵ��£�����̨������̨����	
��������																												ch3���Ĵ�ת�١�ch1����
																																����������Զ�������׼���ڶ��η���
																																
���ȼ����ã�Tim6��7��us��ms��ʱ0 0 ��0 1
						���ܲ�1 0
						TIM2  ������   1 1 
						can���ա����䣺0 0��1 0
																																
																																
																																
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
u8 Auto_Ctrl_Flag;
extern int CurrentSend[MotorNum];
extern u8 us10_on_off;

extern u32 Triggercnt1,us10cnt,Triggercnt2;//����Ϊ�����أ�ż��Ϊ�½���
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
	Motor_Putter_Config();//�Ƹ˳�ʼ��
	Solenoid_Config();//���߹ܣ���ŷ�����ʼ��
	TIM2_Config();
	TIM5_Cap_Init();
	TIM7_Configration();//���뼶��ʱ����
	delay_ms(1000);
	GPIO_Photocell_1();//��������
	GPIO_Photocell_2();
/************ͨ��***************/	
	USART_Config();//���ڳ�ʼ��
	CAN1_Config();//��̨3508
	delay_ms(1000);
///************������ʼ��***************/
	ZeroCheck_Init();
	//Pid_Speed_Init();
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
	{
		Auto_Ctrl_Flag=0;//�ֶ�����
		Gimbal_Power_Flag=0;
	}
/***************Systick********************/
	SysTick_Configuration();
	while(1)
	{
		if(!Photocell_Detect(1))//��⵽������
		{	
			Auto_Ctrl_Flag=1;//��ʼ�Զ�����
			Putter_Back();//��ⷢ����Զ��˻�
//			Auto_Ctrl_Flag=0;
		}
		
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
	
//	Disconnect_Flag_All = Offline_Detect(&Discnect_Flag);//��ȡ������Ϣ,����ͨ���۲���������жϣ�
//	Get_Encode_Speed();//Linear_Velocity�õ�����ٶȣ�
										//Encoder_Speed����ܹ�ת�˼�Ȧ???
	ZeroCheck_cal();//�����λ�û������⣬����Motor_Angle[i]
	ZeroCheck_Excution();//��������������λ�ã����ΪRealPos[i]
	
///**************����״̬����*****************************/
	
//	Disconnect_Report(Disconnect_Flag_All);//led��ӳ����״̬
	Remote_Convert(rc_ctrl);//ͨ��ң�ؿ��Ƽ���/ң��/����״̬�л�������s2�ظ��ã���������ע��
	
///***************ң��ʵ�ʿ��ƺ���*************************/
	
	Status_Act(Status);//״̬�������ã���������̨���������ʵ�ʿ���(����ң�ء�����)
	
///*********************�������**************************/
	
	Pid_Gimbal_Cal_Rc();//��̨λ�û����ٶȻ������������pitch��yaw��pid����

if(RechargeStartOver)//������ʼ������־λ 1���Կ�ʼ��0�Ѿ�����
	{
		Recharge_Task();//������ʼ//�ع��ֶ�����
	}
	//�Ĵ�ת�ٿ���
if(!Auto_Ctrl_Flag)
	{
		testspeed = (rc_ctrl.rc.ch3 - 1024) * 5.0f;//Motor_Cal�Ѿ�����
		Motor_Speed_Set(testspeed,1);
	}

	{
		Gimbal_Current_Send(CurrentSend[0],0,CurrentSend[1],0);//201�ǻ�����203��yaw
	}
	
//		if(rc_ctrl.rc.s2 == 1)
//		{
//			PushToTrack = 1;
//		}
//		else
//		{
//			PushToTrack = 0;
//		}
//	}

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
  NVIC_SetPriority(SysTick_IRQn, 0x05);                       //SysTick�ж����ȼ�����
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

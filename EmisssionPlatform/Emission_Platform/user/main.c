#include "main.h"
/*
引脚：
PC6,7:履带电机
PA0,1:编码器
PA2：光电管1接收管3.3V供电
PA3：光电管1接收管信号通道
PC10：光电管2接收管3.3V供电
PC11：光电管2接收管信号通道
若干气缸
*/
u32 syscnt;
u32 Disconnect_Flag_All;//掉线观察标志位
Disconnect_Flag Discnect_Flag;//断线结构体
Status_ Status;
Lauching_States LauchState;//履带部分
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
/***********基础外设***********/
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
/************通信***************/	
	USART_Config();
	CAN1_Config();//云台3508
	delay_ms(1000);
///************参数初始化***************/
	ZeroCheck_Init();
	Pid_Speed_Init();
	Pid_Gimbal_Pos_Init();
	Pid_Gimbal_Speed_Init();
	rc_reset();
	//???????在光电管初始化之后会直接进且只进一次中断？？？？开关24v电也会导致进一次外部中断
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
///**********************计数器***************************/
	syscnt ++;
///********************传感更新***************************/
////	Disconnect_Flag_All = Offline_Detect(&Discnect_Flag);//读取断线信息
	Get_Encode_Speed();
//	ZeroCheck_cal();
//	ZeroCheck_Excution();
///**************任务状态更新*****************************/
////	Disconnect_Report(Disconnect_Flag_All);//led反映状态
//	Remote_Convert(rc_ctrl);
//	Status_Act(Status);
///*********************计算输出**************************/
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
//	Motor_Speed_Set(testspeed,1);//解锁电机，避免吵
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
  * @brief  Systick初始化
  * @param  None
  * @retval None
  */
void SysTick_Configuration(void)
{
  /* Setup SysTick Timer for 10 msec interrupts  */
  if (SysTick_Config(72000000/ 1000))                //SysTick配置函数 T = 1 / 1000 = 1ms
  { 
    /* Capture error */ 
    while (1);
  }  
 /* Configure the SysTick handler priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  NVIC_SetPriority(SysTick_IRQn, 0x0);                       //SysTick中断优先级设置
}
/**
  * @brief  获取u32的第n位(掉线标志用)
  * @param  num32：u32对象
	*					n:第n位
  * @retval u32的第n位
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
  * @brief  获取运行时间
  * @param  None
  * @retval 系统运行时间ms
  */
u32 GetSyscnt(void)
{
	return syscnt;
}

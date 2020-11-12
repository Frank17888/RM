#include "main.h"
/*
引脚：
PC6,7:履带电机																					遥控器模型： s2														s1
PA0,1:释放舵机																														ch3								ch1
PA2：光电管1接收管3.3V供电																										ch2								ch0
PA3：光电管1接收管信号通道																									与遥控器上s2、s1位置相反
//PC10：光电管2接收管3.3V供电																			s2从上到下：键鼠、摇杆、掉电
//PC11：光电管2接收管信号通道																			s1从上到下：调云台、锁云台、无	
若干气缸																												ch3控履带转速、ch1拨弹
																															拨弹发射后自动换弹，准备第二次发射
																																
优先级配置：Tim6，7作us，ms延时0 0 ，0 1
						光电管测1 0
						TIM2  编码器   1 1 
						can接收、发射：0 0，1，0		
电机数组0,1,2 分别对应 PushPull、Recharge、yaw
*/			
/*
10.17日 debug时可以直接watch CurrentSend[0]观察换弹电机的电流发送0x201
*/
u32 syscnt;
u32 Disconnect_Flag_All;//掉线观察标志位
Disconnect_Flag Discnect_Flag;//断线结构体
Status_ Status;
Lauching_States LauchState;//履带部分
RCC_ClocksTypeDef rcc;
u8 AutoCtrl_Flag;
extern int CurrentSend[MotorNum];
extern u32 Triggercnt,us10cnt;//奇数为上升沿，偶数为下降沿
extern u8 RechargeWork_Flag;

int main()
{
  RCC_GetClocksFreq(&rcc);
/*--------------------------基础外设-----------------------------*/
	LED_Config();
	MG995_TIM5_Config();//拦截舵机初始化
	Pitch_TIM3_Config();//pitch推杆电机初始化
	Motor_Putter_Config();//推杆初始化
	TIM2_Config();//配合光电管10us计时

	TIM7_Config();//毫秒级延时配置
	delay_ms(1000);
	GPIO_Photocell();//光电管配置

/*------------------------------通信------------------------------*/	
	USART_Config();//串口初始化
	CAN1_Config();//云台3508，6020
	delay_ms(1000);
/*---------------------------参数初始化---------------------------*/
	ZeroCheck_Init();
	Pid_Gimbal_Pos_Init();//电机位置环初始化
	Pid_Gimbal_Speed_Init();//电机速度环初始化
	rc_reset();
	Flag_Initialize();
	SysTick_Configuration();
//	//???????在光电管初始化之后会直接进且只进一次中断？？？？开关24v电也会导致进一次外部中断
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
/*----------------------------------计数器----------------------------------*/
	syscnt ++;
/*-----------------------查看是否进入倒回换弹任务-----------------------------------*/
	CheckAutoCtrl();
	PullWork_Task();
	Recharge_Task();
/*-----------------------------系统检测任务-----------------------------------*/
	Disconnect_Flag_All = Offline_Detect(&Discnect_Flag);//读取断线信息,可以通过观察这个变量判断？
	Disconnect_Report(Disconnect_Flag_All);//led反映掉线状态
	ZeroCheck_cal();//各电机位置环过零检测，变量 Motor_Angle[i]
	ZeroCheck_Excution();//过零检测输出各电机位置，输出为 RealPos[i]
	Remote_Convert(rc_ctrl);//通过遥控控制键盘/遥控/掉电状态切换函数
	Status_Act(Status);//状态重新作用，不锁定云台，外加外设实际控制(键鼠、遥控、掉电)
/*----------------------------各电机计算输出----------------------------------*/	
	Pid_Gimbal_Cal_Rc();//云台位置环和速度环、换弹电机、pitch轴yaw轴pid控速
/*-------------------------电流电压给定，控制3508、6020------------------------*/
	Gimbal_Current_Voltage_Send(CurrentSend[0],CurrentSend[1],0/*CurrentSend[3]_pitch*/,0,CurrentSend[2],0,0,0);
//yaw轴电机为6020，需要单独控制
}
/**
  * @brief  Systick初始化
  * @param  None
  * @retval None
  */
void SysTick_Configuration(void)
{
  /* Setup SysTick Timer for 5 msec interrupts  */
  if (SysTick_Config(72000000/ 2000))                //SysTick配置函数 T = 1 / 2000 = 0.5ms
  { 
    /* Capture error */ 
    while (1);
  }  
 /* Configure the SysTick handler priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  NVIC_SetPriority(SysTick_IRQn, 0x00);                       //SysTick中断优先级设置
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

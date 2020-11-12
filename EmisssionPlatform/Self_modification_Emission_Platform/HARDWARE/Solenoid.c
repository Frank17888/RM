#include "main.h"
u8 Solenoid_Power_Flag = 1;
u8 FirstTime = 1;
u8 Recharge_1st=1,Recharge_2nd=1,Recharge_3rd=1;//标志是在换上第几个飞镖
u8 Recharge_1st_Get=0,Recharge_2nd_Get=0,Recharge_3rd_Get=0;
u8 RechargeStartOver ;//标志换弹开始结束，换弹结束是0，可以开始是1
int delay = 1000;
extern u8 Auto_Ctrl_Flag;
extern u32 Triggercnt2;
extern int PutterPosInit;
extern float RealPos[MotorNum];
/**
  * @brief  电磁阀配置
  * @param  None
  * @retval None
  */
void Solenoid_Config(void)
{
	GPIO_InitTypeDef gpio;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
}
/**
  * @brief  倒回动作
  * @param  None
  * @retval None
  */
void Putter_Back(void)
{
	if(!RechargeStartOver)
	{
		//如果上部光电管检测到倒回------------发出0倒回1
		if(Photocell_Detect(1));
		{
			Motor_Speed_Set(2000,0);	//履带电机反转
			PushToTrack = 0;					//推弹气缸退回
			delay_ms(2000);
			//ReceiveUnlock;						//接收锁解锁 

			Motor_Speed_Set(0,1);				//履带电机停转
			RechargeStartOver = 1;//倒回结束，可以开始换弹
		}
		//如果下部光电管检测到倒回
//		if(Photocell_Detect(2));//这是检测到倒回
//		{
//			delay_ms(delay);
//			Motor_Speed_Set(0,1);				//履带电机停转
//			RechargeStartOver = 1;//倒回结束，可以开始换弹
//		}
		
		
	}
}

/**
  * @brief  换弹动作
  * @param  None
  * @retval None
  */
void Recharge_Task(void)
{
		if(Recharge_1st)//第一次换弹是单程，后两次是往返双程。
		{
			Recharge_Pos_Move(Pos1);//换弹电机位置环直接向下移动
			if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//现在位置和目标位置角度相差很小，说明换弹完成
			{
				RechargeStartOver = 0;//换弹结束
				Recharge_1st = 0;
				Auto_Ctrl_Flag=0;
			}
		}
		
		
		
		else if(Recharge_2nd)
		{
			if(Recharge_2nd_Get == 0)
			{
				Recharge_Pos_Move(Pos3);

				Recharge_2nd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos3);//位置移动完成返回1
			}
			else
			{
				Recharge_Pos_Move(Pos1);

				if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//电机移动换弹完成
				{
					Recharge_2nd = 0;
					RechargeStartOver = 0;//换弹结束
					Auto_Ctrl_Flag=0;
				}
			}
		}
		
		else if(Recharge_3rd)
		{
			if(Recharge_3rd_Get == 0)
			{
				Recharge_Pos_Move(Pos4);

				Recharge_3rd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos4);//位置移动完成返回1
			}
			else
			{
				Recharge_Pos_Move(Pos1);

				if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//电机移动换弹完成
				{
					Recharge_3rd = 0;
					RechargeStartOver = 0;//换弹结束
					Auto_Ctrl_Flag=0;
				}
			}
		}

}


/**
  * @brief  范围判定函数
	* @param  Range:单边范围长度
	*					Actual：实际值
	*					Target:目标值
	* @retval 1:进入范围;0:不在范围内
  */
int Range_In(float Range,float Actual,float Target)
{
	if(ABS(Actual-Target) <= Range)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


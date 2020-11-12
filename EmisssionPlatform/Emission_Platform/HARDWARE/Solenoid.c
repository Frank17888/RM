#include "main.h"
u8 Solenoid_Power_Flag = 1;
u8 FirstTime = 1;
u8 Recharge_2nd=1,Recharge_3rd=1,Recharge_4th=1;//标志是在换上第几个飞镖
u8 Recharge_1st_Get,Recharge_3rd_Get,Recharge_4th_Get;
u8 RechargeStartOver ;//标志换弹开始结束
int delay = 1000;

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
  * @brief  换弹动作
  * @param  None
  * @retval None
  */
void Reload_Task(void)
{
	if(Solenoid_Power_Flag == 1)
	{
		
	}
	else
	{
		
	}
}
/**
  * @brief  发射&倒回动作
  * @param  None
  * @retval None
  */
void Test_Task(void)
{
	if(!RechargeStartOver)
	{
		LockMissle;
		delay_ms(3000);
		UnlockMissle;
		ReceiveLock;
		Motor_Speed_Set(2000,1);
		delay_ms(3000);
		PushToTrack = 1;
		//如果上部光电管检测到发出
		while(Photocell_Detect(1));
		{
			Motor_Speed_Set(2000,0);
			PushToTrack = 0;
			delay_ms(2000);
			ReceiveUnlock;
		}
		//如果下部付昂电管检测到倒回
		while(Photocell_Detect(2) == 0);
		{
			delay_ms(delay);
			LockMissle;
		}
		Motor_Speed_Set(0,1);
		RechargeStartOver = 1;//发射结束，可以开始换弹
	}
}

/**
  * @brief  换弹动作
  * @param  None
  * @retval None
  */
void Recharge_Task(void)
{
		if(Recharge_2nd)//第一次换弹是单程，后两次是往返双程。
		{
			Recharge_Pos_Move(PutterPosInit+Pos1);
			if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))
			{
				RechargeStartOver = 0;//换弹结束
				Recharge_2nd = 0;
			}
		}
		else if(Recharge_3rd)
		{
			if(Recharge_3rd_Get == 0)
			{
				Putter_Move_Updown(1);
				Recharge_Pos_Move(PutterPosInit+Pos3);
				Recharge_3rd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos3);
			}
			else
			{
				Putter_Move_Updown(0);
				Recharge_Pos_Move(PutterPosInit+Pos1);
				if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))
				{
					Recharge_3rd = 0;
					RechargeStartOver = 0;//换弹结束
				}
			}
		}
		else if(Recharge_4th)
		{
			if(Recharge_4th_Get == 0)
			{
				Putter_Move_Updown(1);
				Recharge_Pos_Move(PutterPosInit+Pos4);
				Recharge_4th_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos4);
			}
			else
			{
				Putter_Move_Updown(0);
				Recharge_Pos_Move(PutterPosInit+Pos1);//底座处
				if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))
				{
					Recharge_4th = 0;
					RechargeStartOver = 0;//换弹结束
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


#include "main.h"
u8 Solenoid_Power_Flag = 1;
u8 FirstTime = 1;
u8 Recharge_2nd=1,Recharge_3rd=1,Recharge_4th=1;//��־���ڻ��ϵڼ�������
u8 Recharge_1st_Get,Recharge_3rd_Get,Recharge_4th_Get;
u8 RechargeStartOver ;//��־������ʼ����
int delay = 1000;

extern u32 Triggercnt2;
extern int PutterPosInit;
extern float RealPos[MotorNum];
/**
  * @brief  ��ŷ�����
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
  * @brief  ��������
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
  * @brief  ����&���ض���
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
		//����ϲ����ܼ�⵽����
		while(Photocell_Detect(1));
		{
			Motor_Speed_Set(2000,0);
			PushToTrack = 0;
			delay_ms(2000);
			ReceiveUnlock;
		}
		//����²�������ܼ�⵽����
		while(Photocell_Detect(2) == 0);
		{
			delay_ms(delay);
			LockMissle;
		}
		Motor_Speed_Set(0,1);
		RechargeStartOver = 1;//������������Կ�ʼ����
	}
}

/**
  * @brief  ��������
  * @param  None
  * @retval None
  */
void Recharge_Task(void)
{
		if(Recharge_2nd)//��һ�λ����ǵ��̣�������������˫�̡�
		{
			Recharge_Pos_Move(PutterPosInit+Pos1);
			if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))
			{
				RechargeStartOver = 0;//��������
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
					RechargeStartOver = 0;//��������
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
				Recharge_Pos_Move(PutterPosInit+Pos1);//������
				if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))
				{
					Recharge_4th = 0;
					RechargeStartOver = 0;//��������
				}
			}
		}
}


/**
  * @brief  ��Χ�ж�����
	* @param  Range:���߷�Χ����
	*					Actual��ʵ��ֵ
	*					Target:Ŀ��ֵ
	* @retval 1:���뷶Χ;0:���ڷ�Χ��
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

